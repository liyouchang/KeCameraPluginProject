#include "UpdateProtocal.h"
#include "KeUpdateMessage.h"
#include <QBuffer>

UpdateProtocal::UpdateProtocal() :
    ProtocalProcess()
{
}

void UpdateProtocal::ExtractMessage(QByteArray &allBytes, Channel *parser)
{
    int nRead= 0;
    QBuffer buffer(&allBytes);
    buffer.open(QIODevice::ReadOnly);
    const int headLen = sizeof(KEUpdateMsgHead);
    while(!buffer.atEnd())
    {
        if (msgRecv.isEmpty())//上一个消息已经读取完成
        {
            nRead = buffer.read(headBuf+bufPos,headLen-bufPos);
            if(nRead < headLen-bufPos)//消息头在最后几个字节，记录读取的字节，下次继续读取。
            {
                qDebug()<<"Continue Read head in new package\r\n ";
                bufPos = nRead;
                break;
            }
            KEUpdateMsgHead * head = (KEUpdateMsgHead *)headBuf;
            if (head->protocal != PROTOCOL_HEAD||  head->msgLength>msgMaxLen)
            {
                qWarning()<<"The message Protocal Head error, Clear the recv buffer!\r\n";
                msgRecv.clear();
                break;
            }
            msgRecv.resize(head->msgLength + headLen);
            bufPos = 0;
            memcpy(msgRecv.data(),headBuf,headLen);
            bufPos += headLen;
            toRead =  head->msgLength;
            if (toRead != 0)//防止 headLen 越界
            {
                nRead = buffer.read(msgRecv.data()+bufPos, toRead);
                bufPos += nRead;
                toRead -= nRead;
            }
        }
        else//上一个消息未完成读取
        {
            nRead = buffer.read(msgRecv.data()+bufPos,toRead);
            if (nRead < toRead){
                qDebug()<<"to read more and more!";
            }
            bufPos += nRead;
            toRead -= nRead;
        }
        if(toRead == 0 && bufPos == msgRecv.size())//全部读取
        {
            parser->OnRespond(msgRecv);
            msgRecv.clear();
            bufPos = 0;
        }
    }
}
