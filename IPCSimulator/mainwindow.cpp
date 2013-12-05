#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTime>
#include <QDataStream>
#include "Simulator/DevSimulateIPC.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    inst = InstanceController::CreateInstance(KE_IPC_Simulator);
    simulator = SimulatorController::CreateInstance(inst);
    reader = new VideoFileReader(this);

}

MainWindow::~MainWindow()
{
    delete reader;
    delete inst;
    delete ui;
}

void MainWindow::ErrorMessage(const QString &str, int error)
{
    ui->textEdit->showErrorMessage(str,error);
}

void MainWindow::on_btnLogin_clicked()
{

    QString devNum =  this->ui->editDevNum->text();
    int num = devNum.toInt();
    QString svrIp = this->ui->editServerIP->text();
    std::string strSvrIp = svrIp.toLatin1().data();

    QString devMac = this->ui->editMac->text();
    bool ok;
    qlonglong mac = devMac.toLongLong(&ok,16);


    for(int i = 0;i<num;i++){
        SimulatorController * sc = SimulatorController::CreateInstance(inst);
        int ret = sc->ConnectDevice(strSvrIp,22616);
        if(ret != 0){
            this->ErrorMessage(QString("connect server error %1,number %2")
                               .arg(ret).arg(i),EMsg_Error);
            return;
        }
        QString tmpMac = QString::number(mac+i,16);
        while(tmpMac.size() < 12){
            tmpMac.push_front('0');
        }
        std::string strDevMac = tmpMac.toLatin1().data();
        sc->SetMac(strDevMac);
        QThread::msleep(200);
        ret = sc->LoginServer("system","123456");
        if(ret != 0){
            ret = sc->LoginServer("system","123456");
            if(ret != 0){
                sc->DisConnect();
                this->ErrorMessage(QString("login server error %1 -- Device %2,number %3")
                                   .arg(ret).arg(tmpMac).arg(i),EMsg_Error);
                return;
            }
        }
        this->ErrorMessage(QString("login server success -- Device %1,number %2")
                           .arg(tmpMac).arg(i),EMsg_Info);

        DevSimulateIPC * ipc = dynamic_cast<DevSimulateIPC *>(sc);
        if(ipc){
            QObject::connect(reader,&VideoFileReader::sendFileData,
                             ipc,&DevSimulateIPC::SendMediaData);
        }
    }

    //    int ret = simulator->ConnectDevice(strSvrIp,22616);
    //    if(ret != 0){
    //        this->ErrorMessage("connect server error",EMsg_Error);
    //        return;
    //    }

    //    simulator->SetMac(strDevMac);
    //    ret = simulator->LoginServer("system","123456");
    //    if(ret != 0){
    //        simulator->DisConnect();
    //        this->ErrorMessage(QString("login server error %1 -- Device %2").arg(ret).arg(devMac),EMsg_Error);
    //        return;
    //    }
    //    this->ErrorMessage(QString("login server success -- Device %1").arg(devMac),EMsg_Info);

    //    DevSimulateIPC * ipc = dynamic_cast<DevSimulateIPC *>(simulator);
    //    if(ipc){
    //        QObject::connect(reader,&VideoFileReader::sendFileData,
    //                         ipc,&DevSimulateIPC::SendMediaData);
    //    }
    //
}

void MainWindow::on_btnVideo_clicked()
{
    //reader->ipc = simulator;
    QObject::connect(reader,&VideoFileReader::errorMessage,this,&MainWindow::ErrorMessage);
    reader->start();
}
