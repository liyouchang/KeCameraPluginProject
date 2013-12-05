#ifndef DLGSEARCHDEVICE_H
#define DLGSEARCHDEVICE_H

#include <QDialog>
#include "kenetsdk.h"
#include <QUdpSocket>
#include <QStandardItemModel>
#include "IPCHandler.h"
namespace Ui {
class DlgSearchDevice;
}

class DlgSearchDevice : public QDialog
{
    Q_OBJECT

public:
    explicit DlgSearchDevice(QWidget *parent = 0);
    ~DlgSearchDevice();
    void addModelRow(int index,NET_DEVICEINFO * devInfo);
    IPCHandler * pSdkInst;
    QStandardItemModel *model;
private slots:
    void on_btnStartSearch_clicked();

    void on_btnStopSearch_clicked();

private:
    Ui::DlgSearchDevice *ui;
};

#endif // DLGSEARCHDEVICE_H
