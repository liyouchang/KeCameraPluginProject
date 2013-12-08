#ifndef UPDATESERVERWINDOW_H
#define UPDATESERVERWINDOW_H

#include <QMainWindow>
#include "UpdateServer.h"
#include "UpdateDevModel.h"

namespace Ui {
class UpdateServerWindow;
}

class UpdateServerWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit UpdateServerWindow(QWidget *parent = 0);
    ~UpdateServerWindow();
    UpdateServer * m_update;
    UpdateDevModel *model;
    QList<void *> freshDevice;
    QByteArray Fliebuf;
private:
    Ui::UpdateServerWindow *ui;

public slots:
    void  HaveNewConnection(ConnectionController * c);
    void DeviceStatus(void * dev,QString devMac,QString version,QString puType);
    void UpdateProcess(void * dev, int percent);
    void RemoveDevice(void * dev);
private slots:
    void on_pushButton_2_clicked();    
    void on_ChoseFileBtn_clicked();
    void on_btnSelectUpdate_clicked();
};

#endif // UPDATESERVERWINDOW_H
