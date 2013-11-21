#include "VideoFileReader.h"
#include "Simulator/SimulatorController.h"
#include "Simulator/DevSimulateIPC.h"
#include <QDir>
#include <QFile>
#include <QDebug>
#include "mainwindow.h"
#include "KeProtocal/KeMessage.h"
VideoFileReader::VideoFileReader(QObject *parent) :
    QThread(parent)
{
    this->toStop = false;
    this->ipc = 0;
}

void VideoFileReader::NewSimulater(ConnectionController *parent)
{
    Device * pDev = dynamic_cast<Device *>(parent);
    if(pDev == 0){
        qDebug("New connection is not a Device");
        return;
    }
    //DevSimulateIPC * newIPC = new DevSimulateIPC(pDev);
    //QObject::connect(this,&VideoFileReader::fileData,newIPC,&DevSimulateIPC::SendMediaData);
}

void VideoFileReader::fileReader()
{
    QString filePath = QDir::currentPath() + "/video.h264";
    QFile * file = new QFile(filePath);
    qDebug()<<file->fileName();
    if(!file->open(QIODevice::ReadOnly)){
        emit errorMessage("open file error",EMsg_Error);
        return;
    }
    QByteArray fileData = file->readAll();
    emit errorMessage(QString("read file %1 success,file data size is %2.").
                      arg(filePath).arg(fileData.size()),EMsg_Info);
    char * videobuf = fileData.data();
    int bufPos = 0;
    int lastFrameNo = 0;
    while(!toStop){
        if(bufPos >= fileData.size() - 100){
            bufPos = 0;
        }
        KEFrameHead * pHead = (KEFrameHead *)(videobuf + bufPos);
        int frameLen = pHead->frameLen + sizeof(KEFrameHead);
        if(ipc != 0){
          ipc->SendMediaData(1,videobuf + bufPos,frameLen);
        }
        //emit sendFileData(1,videobuf + bufPos,frameLen);
        if(lastFrameNo == 0){
            lastFrameNo = pHead->frameNo;
        }
        //sleep 40 millisecond per frame , 25 frame in one second;
        if(pHead->frameNo != lastFrameNo){
            //qDebug()<<QString("frame %1 sended.").arg(lastFrameNo);
            //emit errorMessage(QString("frame %1 sended.").arg(lastFrameNo),EMsg_Info);
            QThread::msleep(40);
            lastFrameNo = pHead->frameNo;

        }
        bufPos += frameLen;
    }

}

void VideoFileReader::run()
{
    fileReader();
}
