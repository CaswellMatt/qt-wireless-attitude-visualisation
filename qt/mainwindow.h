#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "bleinterface.h"
#include "glwidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
  Q_OBJECT
private:
  Ui::MainWindow *ui;
  GLWidget* m_glWidget;
  QTimer* m_timer;

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();
  BLEInterface* m_bleInterface;

public slots:
  void updateStatusBar(QString info, bool);
  void updateDeviceList(QStringList devices);
  void connectDevice();
  void updateServicesList(QStringList services);
  void dataReceived(QByteArray data);


private slots:
  void handleServiceComboBoxUpdated(int index);
  void handleSendButtonClicked();

private:
  void setupBLEInterface();


};
#endif
