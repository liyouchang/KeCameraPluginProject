#include "ChSecretKey.h"
#include "KeMessage.h"
#include "SocketHandler.h"

ChSecretKey::ChSecretKey(Channel *parent) :
    Channel(parent)
{
}

void ChSecretKey::OnRespond(QByteArray &msgData)
{
    unsigned char  msgType = msgData[1];
    switch(msgType)
    {
    case Client_GetKeyDevice:
    {
        KEDevClientGetKeyResp * pMsg = (KEDevClientGetKeyResp *)msgData.data();
        if(pMsg->videoID != this->getChannelID())
        {
            return;
        }
        this->setClientID(pMsg->clientID);
        m_skey = QByteArray(pMsg->key,8);
        this->wakeup();
    }
        break;
    default:break;
    }
}

int ChSecretKey::Request()
{
    if(!this->m_socketHandle->isValid()){
        return KE_Network_Invalid;
    }

    QByteArray msgSend;
    int msgLen = sizeof(KEDevMsgViaClient);
    msgSend.resize(msgLen);
    KEDevMsgViaClient * pReqMsg = (KEDevMsgViaClient *)msgSend.data();
    pReqMsg->protocal = PROTOCOL_HEAD;
    pReqMsg->msgType = Client_GetKeyDevice;
    pReqMsg->msgLength = msgLen;
    pReqMsg->videoID = this->getChannelID();
    pReqMsg->clientID = this->getClientID();

    return this->m_socketHandle->WriteData(msgSend);
}
