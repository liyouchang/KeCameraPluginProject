#include "ChDeviceLogin.h"
#include "KeProtocal/KeMessage.h"
#include "SocketHandler.h"
#include <QCryptographicHash>
ChDeviceLogin::ChDeviceLogin(Channel *parent) :
    Channel(parent)
{
}

void ChDeviceLogin::initialize(QString loginName, QString loginPWD, QByteArray key)
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

void ChDeviceLogin::OnRespond(QByteArray &msgData)
{
    unsigned char  msgType = msgData[1];
    switch(msgType)
    {
    case Device_LoginServer:
    {
        char * pData = msgData.data();
        int devID = *(int *)(pData +6);
        if(devID != this->getChannelID())
            return;
        result = *(pData+10);
        this->wakeup();
    }
        break;
    }

}

int ChDeviceLogin::Request()
{
    int iDatalen = 88;
    QByteArray sendData(iDatalen,0);
    char * szData = sendData.data();
    *szData = PROTOCOL_HEAD;
    *(szData+1) = Device_LoginServer;
    *(int *)(szData+2) = iDatalen;
    *(int *)(szData+6) = this->m_channelID;
    memcpy(szData+10,loginName.data(),8);
    memcpy(szData+18,loginPwd.data(),8);
    *(szData+26) = 0;
    *(szData+27) = 1;
    memcpy(szData + 28, this->loginMD5.data(), 16);
    *(int *)(szData+44) = 0;
    *(int *)(szData+48) = 0;
    *(int *)(szData+52) = 0;
    *(int *)(szData+56) = 0x17;
    *(int *)(szData+60) = 0x0146;
    *(int *)(szData+64) = 0;
    char szType[20];
    memset(szType, 0, sizeof(szType));
    char *pType = "KESV0117";
    memcpy(szType, pType, strlen(pType));
    memcpy(szData+68,szType, 16);
    *(short *)(szData+84) = 0x5858;
    *(short *)(szData+86) = 0;
    return this->m_socketHandle->WriteData(sendData);
}
