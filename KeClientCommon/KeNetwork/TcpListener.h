#ifndef TCPLISTENER_H
#define TCPLISTENER_H

#include <QTcpServer>
#include "SocketHandler.h"
class TcpListener : public QTcpServer
{
    Q_OBJECT
public:
    explicit TcpListener(QObject *parent = 0);

signals:
    void newSocketHandler(SocketHandler * socket);
public slots:
    // QTcpServer interface
protected:
    void incomingConnection(qintptr handle);
    //self used signal
signals:
    void sbListenConnect(quint16 port );
public slots:
    void sListenConnect( quint16 port = 0);


};

#endif // TCPLISTENER_H
