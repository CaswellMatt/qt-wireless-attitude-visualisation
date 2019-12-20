#include "bleinterface.h"

BLEInterface::BLEInterface(QObject *parent)
  : QObject(parent),
    m_connected(false),
    m_deviceDiscoveryAgent(0),
    m_control(0),
    m_devices(),
    m_servicesUuid(),
    m_service(0),
    m_readCharacteristic(),
    m_writeCharacteristic(),
    m_writeMode(),
    m_readTimer(0)
{
  m_deviceDiscoveryAgent = new QBluetoothDeviceDiscoveryAgent(this);

  connect(m_deviceDiscoveryAgent, SIGNAL(deviceDiscovered(const QBluetoothDeviceInfo&)),
          this, SLOT(addDevice(const QBluetoothDeviceInfo&)));
  connect(m_deviceDiscoveryAgent, SIGNAL(error(QBluetoothDeviceDiscoveryAgent::Error)),
          this, SLOT(onDeviceScanError(QBluetoothDeviceDiscoveryAgent::Error)));
  connect(m_deviceDiscoveryAgent, SIGNAL(finished()), this, SLOT(onScanFinished()));
}


BLEInterface::~BLEInterface() {
  if (m_deviceDiscoveryAgent != nullptr) {
    m_deviceDiscoveryAgent->stop();
  }

  delete m_deviceDiscoveryAgent;

  if (m_control != nullptr) {
    m_control->disconnectFromDevice();
  }

  delete m_control;

  if (m_readTimer != nullptr) {
    m_readTimer->stop();
  }

  delete m_readTimer;
}


void BLEInterface::addDevice(
    const QBluetoothDeviceInfo& device) {

  m_devices.append(device);

  QList<QBluetoothDeviceInfo>::iterator i;

  QStringList allDeviceNames;
  for (i = m_devices.begin(); i != m_devices.end(); ++i) {
    allDeviceNames.append(i->name());
  }

  emit deviceNamesChanged(allDeviceNames);
}

void BLEInterface::onDeviceScanError(
    QBluetoothDeviceDiscoveryAgent::Error error) {
  if (error == QBluetoothDeviceDiscoveryAgent::PoweredOffError) {
    emit statusInfoChanged("The Bluetooth adaptor is powered off, power it on before doing discovery.", false);
  } else if (error == QBluetoothDeviceDiscoveryAgent::InputOutputError) {
    emit statusInfoChanged("Writing or reading from the device resulted in an error.", false);
  } else {
    emit statusInfoChanged("An unknown error has occurred.", false);
  }
}

void BLEInterface::onScanFinished() {
  if (m_devices.size() == 0) {
    emit statusInfoChanged("No Low Energy devices found", false);
  }
}

void BLEInterface::scanForDevices() {
  m_devices.clear();
  emit deviceNamesChanged(QStringList());
  m_deviceDiscoveryAgent->start(QBluetoothDeviceDiscoveryAgent::LowEnergyMethod);
  emit statusInfoChanged("Scanning for devices...", true);
}

void BLEInterface::stopScanningForDevices() {
  m_deviceDiscoveryAgent->stop();
  emit statusInfoChanged("", true);
}

void BLEInterface::connectCurrentDevice(int deviceIndex) {
  if (m_devices.isEmpty())
    return;

  if (m_control) {
    m_control->disconnectFromDevice();
    delete m_control;
    m_control = 0;
  }

  m_control = new QLowEnergyController(m_devices.at(deviceIndex), this);
  connect(m_control, SIGNAL(serviceDiscovered(QBluetoothUuid)),
          this, SLOT(onServiceDiscovered(QBluetoothUuid)));
  connect(m_control, SIGNAL(discoveryFinished()),
          this, SLOT(onServiceScanDone()));
  connect(m_control, SIGNAL(error(QLowEnergyController::Error)),
          this, SLOT(onControllerError(QLowEnergyController::Error)));
  connect(m_control, SIGNAL(connected()),
          this, SLOT(onDeviceConnected()));
  connect(m_control, SIGNAL(disconnected()),
          this, SLOT(onDeviceDisconnected()));

  connect(m_control, SIGNAL(()),
          this, SLOT(onDeviceDisconnected()));

  m_control->connectToDevice();
}

void BLEInterface::onDeviceConnected() {
  m_servicesUuid.clear();
  setCurrentService(-1);
  emit servicesChanged(QStringList());
  m_control->discoverServices();
}

void BLEInterface::onDeviceDisconnected() {
  updateIsDeviceConnectedWith(false);
  emit statusInfoChanged("Service disconnected", false);
  qWarning() << "Remote device disconnected";
}

void BLEInterface::disconnectDevice() {
  m_readTimer->deleteLater();
  m_readTimer = NULL;

  if (m_devices.isEmpty()) {
    return;
  }

  //disable notifications
  if (m_notificationDescriptor.isValid() && m_service) {
    m_service->writeDescriptor(m_notificationDescriptor, QByteArray::fromHex("0000"));
  } else {
    m_control->disconnectFromDevice();
    delete m_service;
    m_service = 0;
  }
}

void BLEInterface::onControllerError(QLowEnergyController::Error error) {
  emit statusInfoChanged("Cannot connect to remote device.", false);
  qWarning() << "Controller Error:" << error;
}

void BLEInterface::updateIsDeviceConnectedWith(bool connectionStatus) {
  if (connectionStatus != m_connected) {
    m_connected = connectionStatus;
    emit connectedChanged(connectionStatus);
  }
}

void BLEInterface::onServiceDiscovered(const QBluetoothUuid &gatt) {
  Q_UNUSED(gatt)
  emit statusInfoChanged("Service discovered. Waiting for service scan to be done...", true);
}

void BLEInterface::onServiceScanDone() {
  m_servicesUuid = m_control->services();

  if (m_servicesUuid.isEmpty()) {
    emit statusInfoChanged("Can't find any services.", true);
  } else {

    QStringList services;
    foreach (auto uuid, m_servicesUuid) {
      services.append(uuid.toString());
    }

    emit servicesChanged(services);
    m_currentService = -1;// to force call update_currentService(once)
    setCurrentService(0);
    emit statusInfoChanged("All services discovered.", true);
  }
}

void BLEInterface::setCurrentService(int currentService)
{
  if (m_currentService == currentService) {
    return;
  }
  updateCurrentService(currentService);
  m_currentService = currentService;
  emit currentServiceChanged(currentService);
  emit statusInfoChanged("", true);
}

void BLEInterface::updateCurrentService(int currentServiceIndex)
{
  delete m_service;
  m_service = 0;

  if (currentServiceIndex >= 0 && m_servicesUuid.count() > currentServiceIndex) {
    m_service = m_control->createServiceObject(
          m_servicesUuid.at(currentServiceIndex), this);
  }

  if (!m_service) {
    emit statusInfoChanged("Service not found.", false);
    return;
  }

  connect(m_service, SIGNAL(stateChanged(QLowEnergyService::ServiceState)),
          this, SLOT(onServiceStateChanged(QLowEnergyService::ServiceState)));
  connect(m_service, SIGNAL(characteristicChanged(QLowEnergyCharacteristic,QByteArray)),
          this, SLOT(onCharacteristicChanged(QLowEnergyCharacteristic,QByteArray)));
  connect(m_service, SIGNAL(characteristicRead(QLowEnergyCharacteristic,QByteArray)),
          this, SLOT(onCharacteristicRead(QLowEnergyCharacteristic,QByteArray)));
  connect(m_service, SIGNAL(characteristicWritten(QLowEnergyCharacteristic,QByteArray)),
          this, SLOT(onCharacteristicWrite(QLowEnergyCharacteristic,QByteArray)));
  connect(m_service, SIGNAL(error(QLowEnergyService::ServiceError)),
          this, SLOT(serviceError(QLowEnergyService::ServiceError)));

  if (m_service->state() == QLowEnergyService::DiscoveryRequired) {
    emit statusInfoChanged("Connecting to service...", true);
    m_service->discoverDetails();
  } else {
    searchCharacteristic();
  }
}

void BLEInterface::onServiceStateChanged(QLowEnergyService::ServiceState serviceState)
{
  qDebug() << "serviceStateChanged, state: " << serviceState;
  if (serviceState == QLowEnergyService::ServiceDiscovered) {
    searchCharacteristic();
  }
}

void BLEInterface::serviceError(QLowEnergyService::ServiceError serviceError)
{
  qWarning() << "Service error:" << serviceError;
}

void BLEInterface::searchCharacteristic() {
  if (m_service) {

    auto handleWriteCharacteristic = [&](const QLowEnergyCharacteristic& characteristic) {
      m_writeCharacteristic = characteristic;
      updateIsDeviceConnectedWith(true);
      if (characteristic.properties() & QLowEnergyCharacteristic::WriteNoResponse)
        m_writeMode = QLowEnergyService::WriteWithoutResponse;
      else
        m_writeMode = QLowEnergyService::WriteWithResponse;
    };

    auto handleReadCharacteristic = [&](const QLowEnergyCharacteristic& characteristic) {
      m_readCharacteristic = characteristic;
      if (!m_readTimer) {
        m_readTimer = new QTimer(this);
        connect(m_readTimer, &QTimer::timeout, this, &BLEInterface::read);
        const uint16_t READ_INTERVAL_MS = 1;
        m_readTimer->start(READ_INTERVAL_MS);
      }
    };


    foreach (QLowEnergyCharacteristic characteristic, m_service->characteristics()) {
      if (characteristic.isValid()) {
        if (characteristic.properties() & QLowEnergyCharacteristic::WriteNoResponse ||
            characteristic.properties() & QLowEnergyCharacteristic::Write) {
          handleWriteCharacteristic(characteristic);
        } else if (characteristic.properties() & QLowEnergyCharacteristic::Read) {
          handleReadCharacteristic(characteristic);
        }

        m_notificationDescriptor = characteristic.descriptor(
              QBluetoothUuid::ClientCharacteristicConfiguration);

        if (m_notificationDescriptor.isValid()) {
          m_service->writeDescriptor(m_notificationDescriptor, QByteArray::fromHex("0100"));
        }
      }
    }
  }
}

void BLEInterface::read() {
  if (m_service && m_readCharacteristic.isValid())
    m_service->readCharacteristic(m_readCharacteristic);
  delete m_readTimer;
}

void BLEInterface::write(const QByteArray &data)
{
  qDebug() << "BLEInterface::write: " << data;
  if (m_service && m_writeCharacteristic.isValid()) {
    static const uint8_t CHUNK_SIZE = 3;
    if (data.length() > CHUNK_SIZE) {
      int sentBytes = 0;
      while (sentBytes < data.length()) {
        m_service->writeCharacteristic(m_writeCharacteristic,
                                       data.mid(sentBytes, CHUNK_SIZE),
                                       m_writeMode);
        sentBytes += CHUNK_SIZE;
        if (m_writeMode == QLowEnergyService::WriteWithResponse) {
          if (m_service->error() != QLowEnergyService::NoError)
            return;
        }
      }
    } else {
      m_service->writeCharacteristic(m_writeCharacteristic, data, m_writeMode);
    }
  }
}

void BLEInterface::onCharacteristicChanged(const QLowEnergyCharacteristic &c,
                                           const QByteArray &value)
{
  Q_UNUSED(c)
  qDebug() << "Characteristic Changed: " << value;
  emit dataReceived(value);
}

void BLEInterface::onCharacteristicWrite(const QLowEnergyCharacteristic &c,
                                         const QByteArray &value)
{
  Q_UNUSED(c)
  qDebug() << "Characteristic Written: " << value;
}

void BLEInterface::onCharacteristicRead(const QLowEnergyCharacteristic &c,
                                        const QByteArray &value) {
  Q_UNUSED(c)
  qDebug() << "Characteristic Read: " << value;
  emit dataReceived(value);
}
