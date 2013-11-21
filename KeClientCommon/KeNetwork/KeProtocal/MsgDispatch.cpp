#include "MsgDispatch.h"
#include <QBuffer>
#include <QDebug>
#include <QTcpSocket>
#include <QTimer>
#include "KeMessage.h"
#include "Device.h"
#include "SocketHandler.h"
QThread MsgDispatch::msgThread;

MsgDispatch::MsgDispatch(Device *dev)
{
    dispatchDev = dev;
    bufPos = 0;
    toRead = 0;

    //this->moveToThread(this);

    //MsgDispatch::msgThread.start();


}

void MsgDispatch::GetMessageData()
{
    qDebug()<<"GetMessageData thread:" <<QThread::currentThreadId();
    if(!dispatchDev){
        return;
    }
    int nRead= 0;
    QByteArray allBytes =  dispatchDev->getSocketHandle()->ReadAll();

    QBuffer buffer(&allBytes);
    buffer.open(QIODevice::ReadOnly);
    const int headLen = 10;

    while(!buffer.atEnd())
    {
        if (msgRecv.empty())//上一个消息已经读取完成
        {
            nRead = buffer.read(headBuf+bufPos,headLen-bufPos);
            if(nRead < headLen-bufPos)//消息头在最后几个字节，记录读取的字节，下次继续读取。
            {
                qDebug()<<"Continue Read head in new package\r\n ";
                bufPos = nRead;
                break;
            }
            unsigned char  protocal = headBuf[0];
            msgLen = *((int*)&headBuf[2]);
            if (protocal != PROTOCOL_HEAD|| msgLen>msgMaxLen)
            {
                qWarning()<<"The message Protocal Head error, Clear the recv buffer!\r\n";
                msgRecv.clear();
                break;
            }
            msgRecv.resize(msgLen);
            bufPos = 0;
            memcpy(&msgRecv[bufPos],headBuf,headLen);
            bufPos += headLen;
            toRead =  msgLen-headLen;
            if (toRead != 0)//防止 headLen 越界
            {
                nRead = buffer.read(&msgRecv[bufPos], toRead);
                bufPos += nRead;
                toRead -= nRead;
            }
        }
        else//上一个消息未完成读取
        {
            nRead = buffer.read(&msgRecv[bufPos],toRead);
            if (nRead < toRead){
                qDebug()<<"to read more and more!";
            }
            bufPos += nRead;
            toRead -= nRead;
        }
        if(toRead == 0 && bufPos == msgRecv.size())//全部读取
        {
            dispatchDev->OnRespond(&msgRecv[0],msgRecv.size());
            msgRecv.clear();
            bufPos = 0;
        }
    }

}

void MsgDispatch::stopGetMsg()
{
    qDebug("MsgDispatch::stopGetMsg()");
    toStop = true;
}
