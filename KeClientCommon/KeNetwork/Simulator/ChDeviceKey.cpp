#include "ChDeviceKey.h"
#include <QByteArray>
#include "KeProtocal/KeMessage.h"
#include "SocketHandler.h"
ChDeviceKey::ChDeviceKey(Channel *parent) :
    Channel(parent)
{
}

void ChDeviceKey::OnRespond(QByteArray &msgData)
{
    unsigned char  msgType = msgData[1];
    switch(msgType)
    {
    case Device_GetKeyServer:
    {
        char * buf = msgData.data();
        int devID = *(int *)(buf +6);
        this->setChannelID(devID);

        memcpy(this->key,buf+10,8);
        m_skey = QByteArray(this->key,8);
        this->wakeup();
    }
        break;
    }
}

int ChDeviceKey::Request()
{
    QByteArray dataSend;
    dataSend.resize(26);
    char * buf = dataSend.data();
    *buf = (char)0xff;
    *(buf+1) = (char)0xd0;
    *(int *)(buf+2) = 26;
    *(int*)(buf+6)=0;
    memcpy(buf+10,this->mac,16);
    return  this->m_socketHandle->WriteData(dataSend);
}
