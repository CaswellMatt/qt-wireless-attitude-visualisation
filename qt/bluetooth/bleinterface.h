#ifndef BLEINTERFACE_H
#define BLEINTERFACE_H

#include <QObject>
#include <QBluetoothDeviceDiscoveryAgent>
#include <QBluetoothDeviceInfo>
#include <QLowEnergyController>
#include <QLowEnergyService>
#include <QMap>
#include <QTimer>

class BLEInterface : public QObject
{
  Q_OBJECT

private:
  bool m_connected;
  QBluetoothDeviceDiscoveryAgent* m_deviceDiscoveryAgent;
  QLowEnergyController *m_control;
  QList<QBluetoothDeviceInfo> m_devices;

  //services
  QList<QBluetoothUuid> m_servicesUuid;
  QLowEnergyService *m_service;
  int m_currentService;

  //Characteristics
  QLowEnergyDescriptor m_notificationDescriptor;
  QLowEnergyCharacteristic m_readCharacteristic;
  QLowEnergyCharacteristic m_writeCharacteristic;
  QLowEnergyService::WriteMode m_writeMode;
  QTimer *m_readTimer;


  void updateIsDeviceConnectedWith(bool connected);
  void updateCurrentService(int currentServiceIndex);
  void searchCharacteristic();

  void read();


public:
  explicit BLEInterface(QObject *parent = nullptr);
  ~BLEInterface();
   void write(const QByteArray &data);

signals:
  void dataReceived(const QByteArray &data);
  void connectedChanged(bool connected);
  void currentServiceChanged(int currentService);
  void deviceNamesChanged(QStringList devices);
  void servicesChanged(QStringList devices);
  void statusInfoChanged(QString info, bool isGood);

private slots:
  //for scanning
  void onDeviceScanError(
      QBluetoothDeviceDiscoveryAgent::Error error);
  void addDevice(const QBluetoothDeviceInfo& device);
  void onScanFinished();
  void stopScanningForDevices();

  //for devices
  void onDeviceConnected();
  void onDeviceDisconnected();
  void onServiceDiscovered(const QBluetoothUuid &gatt);

  //for services
  void onServiceScanDone();
  void disconnectDevice();
  void onControllerError(QLowEnergyController::Error error);
  void onServiceStateChanged(QLowEnergyService::ServiceState serviceState);
  void serviceError(QLowEnergyService::ServiceError serviceError);

  //for characteristics
  void onCharacteristicChanged(const QLowEnergyCharacteristic &c,
                                             const QByteArray &value);
  void onCharacteristicWrite(const QLowEnergyCharacteristic &c,
                                           const QByteArray &value);
  void onCharacteristicRead(const QLowEnergyCharacteristic &c,
                                          const QByteArray &value);

public slots:
  //for scanning
  void scanForDevices();

  //for devices
  void connectCurrentDevice(int deviceIndex);

  void setCurrentService(int currentServiceIndex);
};

#endif
