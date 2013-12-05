#include "DemoMainWindow.h"
#include "ui_DemonMainWindow.h"
#include "DlgSearchDevice.h"
#include <QDateTime>
DemoMainWindow::DemoMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DemoMainWindow)
{
    ui->setupUi(this);
    ui->videoWall->SetDivision(1);

    sdkInst.sID = KENet_CreateInstance(KE_DevType_08Proxy);
    if(sdkInst.sID < 0){
        this->ErrorMessage("create instance error",EMsg_Error);
        return;
    }

    QObject::connect(this,&DemoMainWindow::sendErrorMessage,ui->textEdit,&KeErrorText::showErrorMessage);
}

DemoMainWindow::~DemoMainWindow()
{
    delete ui;
}

void CALLBACK DisConnected(int sID, char *addr, int port, void * user){
    DemoMainWindow * w = static_cast<DemoMainWindow *>(user);

    w->ErrorMessage(QString("connection  %1 disconnected").arg(sID),EMsg_Info);

}

void DemoMainWindow::on_btnConnect_clicked()
{

    QString addr = ui->editDevIP->text();

    //sdkInst.devId = KENet_TryConnect(sdkInst.sID,addr.toLatin1().data(),22616,0,this);
    //this->ErrorMessage(QString("connect to device(%1,%2),device id %3").
    //                  arg(addr).arg(22616).arg(sdkInst.devId),EMsg_Info);
     int ret = KENet_Connect(sdkInst.sID,addr.toLatin1().data(),22616,0,this);
    if(ret < 0){
        this->ErrorMessage(QString("connect to device(%1,%2),error %3").
                       arg(addr).arg(22616).arg(ret),EMsg_Error);
        return;
    }
    sdkInst.devId = ret;
    this->ErrorMessage(QString("connect to device(%1,%2),device id %3").
                   arg(addr).arg(22616).arg(ret),EMsg_Info);
    NET_DEVICEINFO info;
    int loginResult =KENet_LoginDevice(sdkInst.devId,"system","123456",&info);
    if(loginResult == 0){
        this->ErrorMessage(QString("login success,name %1,channelCount %2")
                           .arg(info.deviceName).arg(info.channelCount),EMsg_Info);
    }
    else{
        this->ErrorMessage(QString("login error %1").arg(loginResult),EMsg_Error);
    }

}


void DemoMainWindow::ErrorMessage(const QString &str, int error)
{
    emit sendErrorMessage(str,error);
}

void DemoMainWindow::on_btnSearchDev_clicked()
{
    DlgSearchDevice * dlg = new DlgSearchDevice(this);
    dlg->pSdkInst = &this->sdkInst;
    dlg->exec();
}

void DemoMainWindow::on_btnInitial_clicked()
{

}

void CALLBACK RealDataCallBack(int realHandle, int dataType,const char *buffer, int bufSize, void * user)
{
    VideoWall * v = static_cast<VideoWall *>(user);
    QByteArray data(buffer,bufSize);
    v->MediaDataRecv(0,dataType,data);
}

void DemoMainWindow::on_btnStartView_clicked()
{
    sdkInst.realDataHandler = KENet_StartRealPlay(
                sdkInst.devId,1,RealDataCallBack,0,ui->videoWall);
    this->ErrorMessage(
                QString("start realPlay real id %1").arg(sdkInst.realDataHandler),
                EMsg_Info);
}

void DemoMainWindow::on_btnStopView_clicked()
{
    KENet_StopRealPlay(sdkInst.realDataHandler);
}

void DemoMainWindow::on_btnPTZUp_pressed()
{
    KENet_PTZControl(sdkInst.realDataHandler,0,2,0);
}

void DemoMainWindow::on_btnPTZUp_released()
{
    KENet_PTZControl(sdkInst.realDataHandler,19,2,0);
}

void DemoMainWindow::on_btnPTZRight_released()
{
    KENet_PTZControl(sdkInst.realDataHandler,19,2,0);
}

void DemoMainWindow::on_btnPTZDown_pressed()
{
    KENet_PTZControl(sdkInst.realDataHandler,1,2,0);

}

void DemoMainWindow::on_btnPTZDown_released()
{
    KENet_PTZControl(sdkInst.realDataHandler,19,2,0);
}

void DemoMainWindow::on_btnPTZLeft_pressed()
{
    KENet_PTZControl(sdkInst.realDataHandler,2,2,0);
}

void DemoMainWindow::on_btnPTZLeft_released()
{
    KENet_PTZControl(sdkInst.realDataHandler,19,2,0);
}

void DemoMainWindow::on_btnPTZRight_pressed()
{
    KENet_PTZControl(sdkInst.realDataHandler,3,2,0);
}


void DemoMainWindow::on_btnStartQueryRecord_clicked()
{
    QDateTime startTime = this->ui->timeStart->dateTime();
    QDateTime endTime = this->ui->timeEnd->dateTime();
    NET_TIME st;
    st.year = startTime.date().year();
    st.month = startTime.date().month();
    st.day = startTime.date().day();
    st.hour = startTime.time().hour();
    st.minute = startTime.time().minute();
    st.second = startTime.time().second();

    NET_TIME et;
    et.year = endTime.date().year();
    et.month = endTime.date().month();
    et.day = endTime.date().day();
    et.hour = endTime.time().hour();
    et.minute = endTime.time().minute();
    et.second = endTime.time().second();
    sdkInst.recordFindHandle =
            KENet_FindFile(sdkInst.devId,1,KE_Terminal_All_Record,st,et,3000);

    this->ErrorMessage(
                QString("start find file, find id %1, start time %2, end time %3")
                .arg(sdkInst.recordFindHandle)
                .arg(startTime.toString("yy-M-d hh:mm:ss"))
                .arg(endTime.toString("yy-M-d hh:mm:ss")),
                EMsg_Info);
}

void DemoMainWindow::on_btnNextRecord_clicked()
{
    //NET_RECORDFILE_INFO record;
    int ret = KENet_FindNextFile(sdkInst.recordFindHandle,&record);
    if(ret != 0){
        if(ret == KE_Record_Find_End){
            this->ErrorMessage(QString("find  file end"),EMsg_Info);
        }else{
            this->ErrorMessage(
                        QString("find next file error %1").arg(ret),
                        EMsg_Warning);
        }
        return ;
    }

    QDateTime startTime(QDate(record.starttime.year,record.starttime.month,record.starttime.day),
                        QTime(record.starttime.hour,record.starttime.minute,record.starttime.second));
    QDateTime endTime(QDate(record.endtime.year,record.endtime.month,record.endtime.day),
                      QTime(record.endtime.hour,record.endtime.minute,record.endtime.second));
    this->ErrorMessage(
                QString("record file No.%1, name %2, size %3, time %4 ---- %5")
                .arg(record.fileNo)
                .arg(record.filename)
                .arg(record.size)
                .arg(startTime.toString("yy-M-d hh:mm:ss"))
                .arg(endTime.toString("yy-M-d hh:mm:ss")),
                EMsg_Info);
}

void DemoMainWindow::on_btnStopQuery_clicked()
{
    KENet_FindClose(sdkInst.recordFindHandle);

}
void CALLBACK DownLoadPosCallBack(int recordHandle, int totalSize, int downLoadSize, void * user){

    DemoMainWindow * w = static_cast<DemoMainWindow *>(user);
    int percent = downLoadSize*100/totalSize;
    if(percent > w->downloadPercent){
        w->downloadPercent = percent;
        w->ErrorMessage(
                    QString("recordHandle:%1, download record totalsize %2, downloaded size %3,percent %4")
                    .arg(recordHandle).arg(totalSize).arg(downLoadSize).arg(percent),EMsg_Info);
    }
}


void DemoMainWindow::on_btnSaveRecord_clicked()
{
    downloadPercent = 0;
    int ret= KENet_DownloadByRecordFile(sdkInst.devId,this->record,"test.vid",DownLoadPosCallBack,this);

    if(ret > 0){
        sdkInst.recordSaveHandle = ret;
        this->ErrorMessage(
                    QString("start download record id %1").arg(sdkInst.recordSaveHandle),
                    EMsg_Info);
    }
    else{
        this->ErrorMessage(
                    QString("start download record error %1").arg(ret),
                    EMsg_Error);
    }
}

void DemoMainWindow::on_btnStopSave_clicked()
{
    KENet_StopDownload(sdkInst.recordSaveHandle);
}

void CALLBACK AlarmCallBack(int alarmHandle,int alarmType, char *pBuf, int dwBufLen, void * user){
    DemoMainWindow * w = static_cast<DemoMainWindow *>(user);
    w->ErrorMessage(
                QString("handle %1 receive alarm typ %2")
                .arg(alarmHandle).arg(alarmType),EMsg_Info);
}

void DemoMainWindow::on_btnStartAlarm_clicked()
{
    int ret = KENet_StartAlarm(sdkInst.devId,AlarmCallBack,this);
    if(ret > 0){
        sdkInst.alarmHandle = ret;
        this->ErrorMessage(
                    QString("start alarm id %1").arg(sdkInst.alarmHandle),
                    EMsg_Info);
    }
    else{
        this->ErrorMessage(
                    QString("start alarm error %1").arg(ret),
                    EMsg_Error);
    }
}

void DemoMainWindow::on_btnStopAlarm_clicked()
{
    KENet_StopAlarm(sdkInst.alarmHandle);
}

void DemoMainWindow::on_btnDisconnect_clicked()
{
    KENet_DisConnect(sdkInst.devId);
}
void CALLBACK NewConnection(int sID, int connectHandle,char * param,int paramLen, void * user)
{
    DemoMainWindow * w = static_cast<DemoMainWindow *>(user);
    w->ErrorMessage(
                QString("NewConnection connectHandle %1, MAC %2").arg(connectHandle).arg(param),
                EMsg_Info);
}
void DemoMainWindow::on_btnStartListen_clicked()
{
    int ret = KENet_StartListen(sdkInst.sID,22616,NewConnection,this);
    this->ErrorMessage(QString("start listen  ret %1").
                       arg(ret),EMsg_Info);
}
