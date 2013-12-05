#include "TcpDeviceHolder.h"
#include "CommunicationThread.h"
#include "KeProtocal/ProtocalProcess.h"
#include "SocketHandler.h"
#include "KeProtocal/KeMessage.h"
#include "KeProtocal/ChSearchDevice.h"
#include "KeProxy/ChReceiveLogin.h"
#include "KeProxy/DevConnectSvr.h"
#include "Controller/DeviceFactory.h"
TcpDeviceHolder::TcpDeviceHolder()
{
    ChannelTread::instance().start();
    this->moveToThread(&ChannelTread::instance());
    this->m_server = 0;
    if(m_server == 0){
        m_server = new TcpListener(this);
        QObject::connect(m_server,&TcpListener::newSocketHandler,
                         this,&TcpDeviceHolder::sGetSocketHandler);
    }
}

TcpDeviceHolder::~TcpDeviceHolder()
{
//    if(m_server)
//        delete m_server;
}

void TcpDeviceHolder::SearchedDev(int devIndex, QByteArray devInfo)
{
    if(this->cbSearchDev){
        NET_DEVICEINFO * info = (NET_DEVICEINFO *)devInfo.data();
        this->cbSearchDev(this->getHandler(),devIndex,info,userSearchDev);
    }
}

void TcpDeviceHolder::sGetSocketHandler(SocketHandler *sh)
{
    qDebug("TcpDeviceHolder::sGetSocketHandler");
    AbstractController * pc = dynamic_cast<AbstractController * >(this);
    DeviceFactory::instance().sCreateConnectionController(pc,sh);
    DevConnectSvr * connectDev = dynamic_cast<DevConnectSvr *>(DeviceFactory::instance().currentControl);
    if(connectDev == 0){
        qDebug("TcpDeviceHolder::sGetSocketHandler controll create error");
        return;
    }
    //emit NewConnection(controller);
}

void TcpDeviceHolder::havaNewLogin(int puid, QByteArray devMac)
{
    qDebug("TcpDeviceHolder::havaNewLogin %d,mac %s",puid,devMac.data());
    emit NewLoginInfo(puid,devMac);
    if(cbNewConnection){
         cbNewConnection(this->getHandler(),puid,devMac.data(),devMac.size(),this->userNewConnection);
    }
}

int TcpDeviceHolder::ListenConnect(int port)
{
    if (!m_server->isListening()) {
        // Set up the peer wire server
        //if (!m_server->listen(QHostAddress::Any, port))
        //    return KE_NETWORK_ERROR;
        emit m_server->sbListenConnect(port);
    }
    return 0;
}

int TcpDeviceHolder::SearchDevice()
{
    ChSearchDevice * ch = this->GetChannel<ChSearchDevice *>();
    if(ch == 0){
        ProtocalProcess *protocal =  new ProtocalProcess();
        SocketHandler * sh = new SocketHandler();
        int ret = sh->CreateSocket(QAbstractSocket::UdpSocket);
        if(ret != KE_SUCCESS){
            return ret;
        }
        //sh->ConnectToHost("255.255.255.255",KE_Terminal_Port);
        sh->ConnectToAddr(QHostAddress::Broadcast,KE_Terminal_Port);
        ch = new ChSearchDevice(sh,protocal,this);
        QObject::connect(ch,&ChSearchDevice::recvDeviceInfo,
                         this,&TcpDeviceHolder::SearchedDev);
    }
    return ch->Request();
}

void TcpDeviceHolder::SetNetParam(NET_PARAM param)
{
    this->netParam = param;
    QList<QObject *> listDev =  this->GetChannelList<QObject *>();
    for (int i = 0; i < listDev.size(); ++i) {
        AbstractController * control = dynamic_cast<AbstractController *>(listDev[i]);
        if(control != 0 )
            control->SetNetParam(param);
    }
}

NET_PARAM TcpDeviceHolder::GetNetParam()
{
    return netParam;
}


