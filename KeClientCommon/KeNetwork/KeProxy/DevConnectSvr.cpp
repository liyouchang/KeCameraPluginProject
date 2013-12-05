#include "DevConnectSvr.h"
#include "TcpDeviceHolder.h"
#include "ProtocalKaerProxy.h"
DevConnectSvr::DevConnectSvr(SocketHandler *s, ProtocalProcess *protocal, Device *parent)
    :DevConnection(s,protocal,parent)
{
    chLogin = new ChReceiveLogin(this);
    chLogin->setChannelID(this->getHandler());
    TcpDeviceHolder * holder = dynamic_cast<TcpDeviceHolder *>(parent);
    if(holder != 0){
        QObject::connect(chLogin,&ChReceiveLogin::getDeviceLogin,holder,&TcpDeviceHolder::havaNewLogin);
    }
}

int DevConnectSvr::LoginServer(std::string username, std::string pwd)
{
    return KE_Not_Support_Function;
}

int DevConnectSvr::Logout()
{
    return KE_Not_Support_Function;
}

void DevConnectSvr::OnRespond(QByteArray &msgData)
{
    this->m_protocal->ParseMessage(msgData,this);
}

void DevConnectSvr::DevDisconnect()
{
    qDebug("DevConnectSvr::DevDisconnect delete connection");
    delete this;

}

int DevConnectSvr::Request()
{
    return 0;
}
