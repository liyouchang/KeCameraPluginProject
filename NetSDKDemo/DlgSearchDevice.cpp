#include "DlgSearchDevice.h"
#include "ui_DlgSearchDevice.h"
#include <QNetworkInterface>
#include <QHeaderView>
DlgSearchDevice::DlgSearchDevice(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgSearchDevice)
{
    ui->setupUi(this);

    ui->deviceView->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    model = new QStandardItemModel(0, 6, parent);
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("No."));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("ip"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("netmask"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("gateway"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("SN"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("soft ver"));

    ui->deviceView->setModel(model);
}

DlgSearchDevice::~DlgSearchDevice()
{
    delete ui;
}

void DlgSearchDevice::addModelRow(int index, NET_DEVICEINFO *devInfo)
{
    model->insertRow(index);
    model->setData(model->index(index, 0), index);
    model->setData(model->index(index, 1), devInfo->szIp);
    model->setData(model->index(index, 2), devInfo->szMask);
    model->setData(model->index(index, 3), devInfo->szGateway);
    model->setData(model->index(index, 4), devInfo->deviceSN);
    model->setData(model->index(index, 5), devInfo->softwareVer);
}
void CALLBACK SearchedDevice(int sID,int index,NET_DEVICEINFO * devInfo,void *pUser){
    //qDebug("searched device %d %d",sID,index);
    DlgSearchDevice * dlg = (DlgSearchDevice *)pUser;
    dlg->addModelRow(index,devInfo);
}

void DlgSearchDevice::on_btnStartSearch_clicked()
{
   int ret =  KENet_StartSearchDevice(pSdkInst->sID,SearchedDevice,this);
   qDebug("search device ret = %d",ret);

}

void DlgSearchDevice::on_btnStopSearch_clicked()
{
    KENet_StopSearchDevice(pSdkInst->sID);
}
