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
};

#endif // TCPLISTENER_H
