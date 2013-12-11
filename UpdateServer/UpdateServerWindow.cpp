#include "UpdateServerWindow.h"
#include "ui_UpdateServerWindow.h"
#include <QMessageBox>
#include <QFile>
#include <QFileDialog>
#include "DevUpdate.h"

#include <QTimer>

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


    model = new UpdateDevModel(headers,this);

    modelProxy = new QSortFilterProxyModel(this);
    modelProxy->setSourceModel(model);
    qDebug("start!");
    modelProxy->setFilterKeyColumn(0);

    ui->devView->setModel(modelProxy);
    ui->devView->header()->setSectionResizeMode(QHeaderView::ResizeToContents);

    //QObject::connect(model,SIGNAL(selectedRows(int)),ui->labelSelectedNum,SLOT(setNum(int)));
    m_update = new UpdateServer;
    QObject::connect(m_update,&UpdateServer::NewConnection,
                     this,&UpdateServerWindow::HaveNewConnection,
                     Qt::BlockingQueuedConnection);
    int ret = m_update->ListenConnect(17000);
    if(ret != 0)
        QMessageBox::critical(this,"启动链接", "开启链接失败");

    ui->comboSearchColumn->addItems(headers);


    QTimer * refreshInfo = new QTimer(this);
    QObject::connect(refreshInfo,&QTimer::timeout,this,&UpdateServerWindow::RefreshInfo);
    refreshInfo->start(300);
}

UpdateServerWindow::~UpdateServerWindow()
{
    m_update->disconnect(SIGNAL(NewConnection(ConnectionController *)));
    m_update->deleteLater();
    delete ui;
}

void UpdateServerWindow::HaveNewConnection(ConnectionController *c)
{
    DevUpdate *dev = dynamic_cast<DevUpdate *>(c);
    if(dev == 0){
        qDebug("UpdateServerWindow::HaveNewConnection case error");
    }
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
        //qDebug()<<"mac:"<<devMac<<" ver:"<<version<<" type:"<<puType;
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
    //qDebug("UpdateServerWindow::UpdateProcess dev:%d, lastPack:%d",dev,percent);
    if(percent <= 100){
        this->model->changeItem(dev,UpdateStatus_Doing,percent);
    }
    else if(percent == 200) //update success
    {
        this->model->changeItem(dev,UpdateStatus_Done,percent);
        this->endUpdate(dev);
    }
    else{
        this->model->changeItem(dev,UpdateStatus_Error,percent);
        this->endUpdate(dev);
    }
}

void UpdateServerWindow::RemoveDevice(void *dev)
{
    //qDebug("delete item %d",dev);
    if(freshDevice.contains(dev)){
        freshDevice.removeOne(dev);
    }
    else{
        this->endUpdate(dev);
        this->model->deleteItem(dev);
    }
}

void UpdateServerWindow::RefreshInfo()
{
    ui->labelTotalDeviceNum->setNum(this->model->itemsCount());
    ui->labelUpdatingNumer->setNum(this->updatingList.size());
    ui->labelWaitingNumber->setNum(this->waitingList.size());

    int selectNum = ui->devView->selectionModel()->selectedRows().size();
    int currentRows = ui->devView->model()->rowCount();
    ui->btnCheckAll->setText(QString("全部选择 （%1）").arg(currentRows));
    ui->btnSelectUpdate->setText(QString("选择升级 （%1）").arg(selectNum));

}

bool UpdateServerWindow::startUpdate(void *dev)
{
    if(this->updatingList.contains(dev) ||
            this->waitingList.contains(dev))
    {
        return false;
    }
    if(this->updatingList.size() < 10){
        this->updatingList.append(dev);
        this->model->changeItem(dev,UpdateStatus_Ready,0);
        DevUpdate *upDev = static_cast<DevUpdate *>(dev);
        upDev->StartDevUpdate(m_fileBuffer);
    }
    else{
        this->waitingList.append(dev);
        this->model->changeItem(dev,UpdateStatus_Waiting,0);
    }
    return true;
}

bool UpdateServerWindow::endUpdate(void *dev)
{
    bool ret = true;
    if(this->updatingList.contains(dev))
    {
        this->updatingList.removeOne(dev);

        if(!this->waitingList.isEmpty()){
            void * oneWait = this->waitingList.takeFirst();
            updatingList.append(oneWait);
            this->model->changeItem(oneWait,UpdateStatus_Ready,0);
            DevUpdate *upDev = static_cast<DevUpdate *>(oneWait);
            upDev->StartDevUpdate(m_fileBuffer);
        }
    }
    else{
        ret =  this->waitingList.removeOne(dev);
    }
    return ret;
}



void UpdateServerWindow::on_ChoseFileBtn_clicked()
{
    QString fileName =  QFileDialog::getOpenFileName(this);
    if(fileName != "")
    {
        ui->editFileName->setText(fileName);
        QFile readfile(fileName);
        readfile.open(QIODevice::ReadOnly);
        m_fileBuffer = readfile.readAll();
    }

}

void UpdateServerWindow::on_btnSelectUpdate_clicked()
{
    if(m_fileBuffer.isEmpty()){
        QMessageBox::warning(this,"升级", "未选择升级文件");
        return;
    }
    QModelIndexList selectRows =  ui->devView->selectionModel()->selectedRows();
    for(int i = 0 ;i < selectRows.size(); ++i)
    {
        QModelIndex oneSelectIndex = selectRows.value(i);
        QVariant v =  ui->devView->model()->data(oneSelectIndex,Qt::UserRole);
        QModelIndex sourceData = v.toModelIndex();
        if(sourceData.isValid()){
            DeviceItem * item = static_cast<DeviceItem *>(sourceData.internalPointer());
            this->startUpdate(item->connect);
            //qDebug()<<"start update mac "<<item->mac<<" number "<<i;

        }
        else{
            QMessageBox::warning(this,"升级", "wrong select");
            return;
        }
    }
    ui->devView->selectionModel()->clear();
}


void UpdateServerWindow::on_editSearch_textChanged(const QString &arg1)
{
    modelProxy->setFilterRegExp(arg1);
}

void UpdateServerWindow::on_comboSearchColumn_currentIndexChanged(int index)
{
    modelProxy->setFilterKeyColumn(index);
}

void UpdateServerWindow::on_btnCheckAll_clicked()
{
    ui->devView->selectAll();
}

void UpdateServerWindow::on_btnNoCheck_clicked()
{
    ui->devView->selectAll();
    QModelIndexList selectRows =  ui->devView->selectionModel()->selectedRows();
    for(int i = 0 ;i < selectRows.size(); ++i)
    {
        QModelIndex oneSelectIndex = selectRows.value(i);
        ui->devView->model()->setData(oneSelectIndex,Qt::Unchecked,Qt::CheckStateRole);
    }
    ui->devView->selectionModel()->clear();

}
