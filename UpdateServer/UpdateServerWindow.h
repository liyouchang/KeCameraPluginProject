#ifndef UPDATESERVERWINDOW_H
#define UPDATESERVERWINDOW_H

#include <QMainWindow>
#include "UpdateServer.h"
#include "UpdateDevModel.h"
#include "QSortFilterProxyModel"
#include <QList>
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
    QSortFilterProxyModel *modelProxy;

    QList<void *> freshDevice;
    QList<void *> updatingList;
    QList<void *> waitingList;
    QByteArray m_fileBuffer;
    bool startUpdate(void *dev);
    bool endUpdate(void *dev);

private:
    Ui::UpdateServerWindow *ui;

public slots:
    void  HaveNewConnection(ConnectionController * c);
    void DeviceStatus(void * dev,QString devMac,QString version,QString puType);
    void UpdateProcess(void * dev, int percent);
    void RemoveDevice(void * dev);
    void RefreshInfo();
private slots:
    void on_ChoseFileBtn_clicked();
    void on_btnSelectUpdate_clicked();
    void on_editSearch_textChanged(const QString &arg1);
    void on_comboSearchColumn_currentIndexChanged(int index);
    void on_btnCheckAll_clicked();
    void on_btnNoCheck_clicked();
};

#endif // UPDATESERVERWINDOW_H
