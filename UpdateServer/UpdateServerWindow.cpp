#include "UpdateServerWindow.h"
#include "ui_UpdateServerWindow.h"
#include <QMessageBox>
#include <QFile>
#include <QFileDialog>
#include "DevUpdate.h"

#if defined(_MSC_VER)
#pragma  execution_character_set("utf-8")
#endif

UpdateServerWindow::UpdateServerWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::UpdateServerWindow)
{
    ui->setupUi(this);

    QStringList headers;
    headers << tr("设备Mac")
            << tr("设备类型")
            << tr("固件版本")
            << tr("升级信息");

    model = new UpdateDevModel(headers);
    ui->devView->setModel(model);
    m_update = new UpdateServer;
    QObject::connect(m_update,&UpdateServer::NewConnection,this,&UpdateServerWindow::HaveNewConnection);
    int ret = m_update->ListenConnect(17000);
    if(ret != 0)
        QMessageBox::critical(this,"启动链接", "开启链接失败");

}

UpdateServerWindow::~UpdateServerWindow()
{
    m_update->deleteLater();
    delete ui;
}

void UpdateServerWindow::HaveNewConnection(ConnectionController *c)
{
    DevUpdate *dev = dynamic_cast<DevUpdate *>(c);
    qDebug("UpdateServerWindow::HaveNewConnection %d",dev);

    QObject::connect(dev,&DevUpdate::DevUpdateStatus,this, &UpdateServerWindow::DeviceStatus);
    QObject::connect(dev,&DevUpdate::DevUpdateProcess,this, &UpdateServerWindow::UpdateProcess);
    QObject::connect(dev,&DevUpdate::DeleteDevice,this, &UpdateServerWindow::RemoveDevice);
    freshDevice << dev;
}

void UpdateServerWindow::DeviceStatus(void *dev, QString devMac, QString version, QString puType)
{
    DeviceItem * item = new DeviceItem();
    item->connect = dev;
    item->mac = devMac;
    item->version = version;
    item->type = puType;
    if(freshDevice.contains(dev)){
        freshDevice.removeOne(dev);
        this->model->pushItem(item);
        ui->labelTotalDeviceNum->setNum(this->model->itemsCount());
    }
    else{
        this->model->changeItem(item);
    }
    delete item;
}

void UpdateServerWindow::UpdateProcess(void *dev, int percent)
{
    qDebug("UpdateServerWindow::UpdateProcess dev:%d, lastPack:%d",dev,percent);
    if(percent < 100)
        this->model->changeItem(dev,UpdateStatus_Doing,percent);
    else
        this->model->changeItem(dev,UpdateStatus_Done,percent);

}

void UpdateServerWindow::RemoveDevice(void *dev)
{
    qDebug("delete item %d",dev);
    if(freshDevice.contains(dev)){
        freshDevice.removeOne(dev);
    }
    else{
        this->model->deleteItem(dev);
        ui->labelTotalDeviceNum->setNum(this->model->itemsCount());
    }
}

void UpdateServerWindow::on_pushButton_2_clicked()
{
    DeviceItem * item = new DeviceItem();
    item->connect = 0;
    item->mac = "devMac";
    item->version = "version";
    item->type = "puType";
    this->model->pushItem(item);
}

void UpdateServerWindow::on_ChoseFileBtn_clicked()
{
    QString fileName =  QFileDialog::getOpenFileName(this);
    if(fileName != "")
    {
        ui->editFileName->setText(fileName);
        QFile readfile(fileName);
        readfile.open(QIODevice::ReadOnly) ;

        Fliebuf = readfile.readAll();
    }

}

void UpdateServerWindow::on_btnSelectUpdate_clicked()
{
    QModelIndexList selectRows =  ui->devView->selectionModel()->selectedRows();

    for(int i = 0 ;i < selectRows.size(); ++i){
        QModelIndex mi = selectRows.value(i);
        DeviceItem * item = static_cast<DeviceItem *>( mi.internalPointer());
        DevUpdate *dev = static_cast<DevUpdate *>(item->connect);
        dev->StartDevUpdate(Fliebuf);

    }
}
