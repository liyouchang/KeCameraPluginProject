#include "TcpListener.h"

TcpListener::TcpListener(QObject *parent) :
    QTcpServer(parent)
{
}

void TcpListener::incomingConnection(qintptr handle)
{
    SocketHandler * s = new SocketHandler();
    s->SetSocketDescriptor(handle);
    emit newSocketHandler(s);

}
