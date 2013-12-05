#include "ChLogin.h"
#include "KeMessage.h"
#include "SocketHandler.h"
#include <QCryptographicHash>
#include <QHostAddress>
ChLogin::ChLogin(Channel *parent) :
    Channel(parent)
{
    this->result = KE_Not_Login;
}

void ChLogin::makeMd5(QString loginName, QString loginPWD, QByteArray key)
{
    this->loginName = loginName.toLatin1();
    int pushSize = 8-this->loginName.size();
    for(int i =0;i< pushSize;i++)
        this->loginName.append(0x20);

    this->loginPwd = loginPWD.toLatin1();
    pushSize = 8-this->loginPwd.size();
    for(int i =0;i< pushSize;i++)
        this->loginPwd.append(0x20);

    QByteArray md5Pwd;
    md5Pwd.append(this->loginName);
    md5Pwd.append(key);
    md5Pwd.append(this->loginPwd);

    QCryptographicHash md(QCryptographicHash::Md5);
    md.addData(md5Pwd);
    this->loginMD5 = md.result();

    qDebug()<<"the md5 string is "<<this->loginMD5.toHex()
           << " size is "<< this->loginMD5.size();
}

void ChLogin::OnRespond(QByteArray &msgData)
{
    unsigned char  msgType = msgData[1];
    switch(msgType)
    {
    case Client_LoginDevice:
    {
        KEDevClientLoginResp * pMsg = (KEDevClientLoginResp *)msgData.data();
        if(pMsg->videoID != this->getChannelID() ||pMsg->clientID != this->getClientID())
        {
            return;
        }
        if(pMsg->resp == RESP_ACK){
            this->result = KE_SUCCESS;
        }
        else{
            this->result = KE_Login_Error;
        }

        CopyIP(this->devInfo.szIp,pMsg->ip);
        CopyIP(this->devInfo.szMask,pMsg->mask);
        CopyIP(this->devInfo.szGateway,pMsg->gateway);
        strcpy(this->devInfo.deviceSN,pMsg->deviceId);
        strcpy(this->devInfo.deviceName,pMsg->terName);
        this->devInfo.hardwareVer = pMsg->hardVer;
        int currentSize = sizeof(KEDevClientLoginResp);
        int channelCount = 0;
        while(pMsg->msgLength > currentSize){
            SChlName * oneCh = (SChlName *)(msgData.data() + currentSize);
            this->devInfo.channelInfoArray[channelCount].enable = oneCh->byStart & 0x01;
            memcpy(this->devInfo.channelInfoArray[channelCount].channelName,oneCh->szName,40);
            if(oneCh->byStart & 0x80){
                this->devInfo.channelInfoArray[channelCount].type = 1;
            }
            else{
                this->devInfo.channelInfoArray[channelCount].type = 0;
            }
            currentSize += sizeof(SChlName);
            ++channelCount;
        }
        devInfo.channelCount = channelCount;

        memcpy(this->devInfo.szUsrName,this->loginName.data(),8);
        memcpy(this->devInfo.szUsrPwd,this->loginPwd.data(),8);
        this->devInfo.iPort = this->m_socketHandle->m_port;

        this->wakeup();
    }
        break;
    default:break;
    }
}

int ChLogin::Request()
{
    if(!this->m_socketHandle->isValid()){
        return KE_Network_Invalid;
    }
    QByteArray msgSend;
    int msgLen = sizeof(KEDevClientLoginReq);
    msgSend.resize(msgLen);
    KEDevClientLoginReq * pReqMsg = (KEDevClientLoginReq *)msgSend.data();
    pReqMsg->protocal = PROTOCOL_HEAD;
    pReqMsg->msgType = Client_LoginDevice;
    pReqMsg->msgLength = msgLen;
    pReqMsg->videoID = this->getChannelID();
    pReqMsg->clientID = this->getClientID();
    memcpy(pReqMsg->md5,this->loginMD5.data(),16);

    return this->m_socketHandle->WriteData(msgSend);
}
