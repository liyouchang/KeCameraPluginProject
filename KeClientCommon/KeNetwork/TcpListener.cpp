#include "TcpListener.h"

TcpListener::TcpListener(QObject *parent) :
    QTcpServer(parent)
{
    QObject::connect(this,&TcpListener::sbListenConnect,
                     this,&TcpListener::sListenConnect);
}

void TcpListener::incomingConnection(qintptr handle)
{
//    qDebug("TcpListener::incomingConnection");
//    SocketHandler * s = new SocketHandler();
//    s->CreateSocket();
//    s->SetSocketDescriptor(handle);
    void * tmp = (void *)handle;
    emit newSocketHandler(tmp);

}

void TcpListener::sListenConnect(quint16 port)
{
    bool ret = this->listen(QHostAddress::Any,port);
    if(!ret){
        qWarning("TcpListener::sListenConnect listen error");
    }
}
