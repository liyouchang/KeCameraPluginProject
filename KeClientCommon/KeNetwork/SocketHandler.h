#ifndef SOCKETHANDLER_H
#define SOCKETHANDLER_H

#include <QObject>
#include <QAbstractSocket>
#include <QHostAddress>
#include <QByteArray>
#include <QMutex>
#include <QMutexLocker>
#include "KeProtocal/MsgDispatch.h"

class SocketHandler : public QObject
{
    Q_OBJECT
public:
    explicit SocketHandler(QObject *parent = 0);
    virtual ~SocketHandler();
    void initailize();
    void cleanUp();
    //0,create tcp socket;1,create udp socket
    int CreateSocket(int socketType = 0);

    int TryConnectToHost(const QString &hostName, quint16 port);
    int ConnectToHost(const QString &hostName, quint16 port,int timeout = 0);
    int ConnectToAddr(const QHostAddress & address, quint16 port,int timeout = 0);
    bool isValid();
    int DisConnectFromHost();
    int WriteData(const QByteArray & byteArray);
    int SetSocketDescriptor(qintptr socketDescriptor);
    QByteArray ReadAll();
    QAbstractSocket * getSocket();
public:
    QAbstractSocket * m_socket;
    int lastError;
    QMutex errorMutex;
    QHostAddress m_addr;
    quint16 m_port;
    int connectTimeout;
    bool toTryConnect;
signals://self connect singnal
    void sbCreateSocket();
    void sbConnectToHost(const QString &hostName, quint16 port);
    void sbConnectToAddr(const QHostAddress & address, quint16 port);

    void sbDisConnectFromHost();
    void sbWriteData(const QByteArray & byteArray);
    void saWriteData(const QByteArray & byteArray);

    void saCleanUp();
    void sbSetSocketDescriptor(qintptr socketDescriptor);
signals:
    void saReadyRead();
    void saDisConnected();
    void saConnected();
    void sdReadedData(QByteArray & byteArray);
    void saReadedData(QByteArray byteArray);

public slots:
    void sCreateSocket();
    void sConnectToHost(const QString &hostName, quint16 port);
    void sConnectToAddr(const QHostAddress & address, quint16 port);

    void sDisConnectFromHost();
    void sWriteData(const QByteArray & byteArray);
    void sReadData();
    void sCleanUp();
    void sSetSocketDescriptor(qintptr socketDescriptor);

protected:
    int m_socketType;
};

#endif // SOCKETHANDLER_H
