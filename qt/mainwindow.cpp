#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGLFormat>
MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent),
    ui(new Ui::MainWindow) {

  ui->setupUi(this);
  setupBLEInterface();

  m_timer = new QTimer(this);
  const int IntervalTime = 1;
  connect(m_timer, SIGNAL(timeout()),
          ui->openGLWidget, SLOT(update()));
  m_timer->start(IntervalTime);

  connect(this, SIGNAL(resizeWindow(QResizeEvent *)),
          ui->openGLWidget, SLOT(resizeGL(QResizeEvent *)));

}

MainWindow::~MainWindow() {
  delete ui;
}

void MainWindow::setupBLEInterface() {
  m_bleInterface = new BLEInterface(this);

  connect(m_bleInterface, SIGNAL(deviceNamesChanged(QStringList)),
          this, SLOT(updateDeviceList(QStringList)));

  connect(m_bleInterface, SIGNAL(servicesChanged(QStringList)),
          this, SLOT(updateServicesList(QStringList)));

  connect(m_bleInterface, SIGNAL(dataReceived(const QByteArray &)),
          this, SLOT(dataReceived(const QByteArray &)));

  connect(m_bleInterface, SIGNAL(statusInfoChanged(QString, bool)),
          this, SLOT(updateStatusBar(QString, bool)));

  connect(ui->bleScanButton, SIGNAL(pressed()),
          m_bleInterface, SLOT(scanForDevices()));

  connect(ui->bleStopScanButton, SIGNAL(pressed()),
          m_bleInterface, SLOT(stopScanningForDevices()));

  connect(ui->connectButton, SIGNAL(pressed()),
          this, SLOT(connectDevice()));

  connect(ui->servicesComboBox, SIGNAL(currentIndexChanged(int)),
          this, SLOT(handleServiceComboBoxUpdated(int)));

  connect(ui->sendButton, SIGNAL(pressed()),
          this, SLOT(handleSendButtonClicked()));


  connect(m_bleInterface, SIGNAL(dataReceived(const QByteArray &)),
          ui->openGLWidget, SLOT(dataReceived(const QByteArray &)));
}

void MainWindow::updateStatusBar(QString info, bool) {
  this->statusBar()->showMessage(info);
}

void MainWindow::updateDeviceList(QStringList devices) {
  ui->devicesComboBox->clear();
  ui->devicesComboBox->addItems(devices);
}

void MainWindow::connectDevice() {
  m_bleInterface->connectCurrentDevice(ui->devicesComboBox->currentIndex());
}

void MainWindow::updateServicesList(QStringList services) {
  ui->servicesComboBox->clear();
  ui->servicesComboBox->addItems(services);
}

void MainWindow::handleServiceComboBoxUpdated(int index)
{
  m_bleInterface->setCurrentService(index);
}
#include <QTextCodec>
void MainWindow::dataReceived(QByteArray data) {

  QTextCodec *codec = QTextCodec::codecForName("UTF-8");
  QString string = codec->toUnicode(data);

  QStringList query;
  query << string.split(" ");

  if (ui->asciiRadioButton->isChecked()) {
    ui->receivedTextEdit->append(data);
  } else {
    ui->receivedTextEdit->append(data.toHex());
  }
}

void MainWindow::handleSendButtonClicked()
{
  QByteArray data;
  if (ui->asciiRadioButton->isChecked()) {
    data =  QByteArray(ui->sendTextEdit->toPlainText().toLatin1());
  } else {
    data = QByteArray::fromHex(ui->sendTextEdit->toPlainText().toLatin1());
  }

  m_bleInterface->write(data);
  ui->sendTextEdit->clear();
}

void MainWindow::resizeEvent(QResizeEvent *event) {
  emit resizeWindow(event);
  QMainWindow::resizeEvent(event);
}



