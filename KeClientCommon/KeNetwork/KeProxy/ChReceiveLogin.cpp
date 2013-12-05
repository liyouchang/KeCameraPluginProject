#include "ChReceiveLogin.h"
#include "KeProtocal/KeMessage.h"
#include "SocketHandler.h"
ChReceiveLogin::ChReceiveLogin(Channel *parent) :
    Channel(parent)
{
}

void ChReceiveLogin::OnRespond(QByteArray &msgData)
{
    unsigned char  msgType = msgData[1];
    switch(msgType)
    {
    case Device_GetKeyServer:
    {
        KeDeviceGetKeyReq * pMsg = (KeDeviceGetKeyReq *)msgData.data();
        this->devMac = QByteArray(pMsg->deviceMac,16);
        qDebug("ChReceiveLogin::OnRespond msg Device_GetKeyServer,mac %s",
               pMsg->deviceMac);

        QByteArray respData;
        int respLen = sizeof(KeDeviceGetKeyResp);
        respData.resize(respLen);
        KeDeviceGetKeyResp *respMsg = (KeDeviceGetKeyResp *)respData.data();
        respMsg->protocal = PROTOCOL_HEAD;
        respMsg->msgType = Device_GetKeyServer;
        respMsg->msgLength = respLen;
        respMsg->videoID = this->getChannelID();
        this->m_socketHandle->sWriteData(respData);
        qDebug("ChReceiveLogin::Device_GetKeyServer sWriteData",msgType);
    }
        break;
    case Device_LoginServer:
    {
        KEDevMsgHead *head = (KEDevMsgHead *)msgData.data();
        if(head->videoID != this->getChannelID()){
            return;
        }

        qDebug("ChReceiveLogin::OnRespond msg Device_LoginServer,vid %d",
               head->videoID);

        QByteArray respData;
        int respLen = sizeof(KeDeviceLoginResp);
        respData.resize(respLen);
        KeDeviceLoginResp *respMsg = (KeDeviceLoginResp *)respData.data();
        respMsg->protocal = PROTOCOL_HEAD;
        respMsg->msgType = Device_LoginServer;
        respMsg->msgLength = respLen;
        respMsg->videoID = this->getChannelID();
        respMsg->resp = RESP_ACK;

        this->m_socketHandle->sWriteData(respData);
        emit getDeviceLogin(this->getChannelID(),this->devMac);
    }
        break;
    default:
        qWarning("ChReceiveLogin receive unsurported message");
        break;
    }
}

int ChReceiveLogin::Request()
{
    return 0;
}
