#ifndef SOCKETHANDLER_H
#define SOCKETHANDLER_H

#include <QObject>
#include <QAbstractSocket>
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
    void CreateSocket();
    int ConnectToHost(const QString &hostName, quint16 port);
    int DisConnectFromHost();
    int WriteData(const QByteArray & byteArray);
    int SetSocketDescriptor(qintptr socketDescriptor);
    QByteArray ReadAll();
    QAbstractSocket * getSocket();
public:
    QAbstractSocket * m_socket;
    int lastError;
    QMutex errorMutex;
signals:
    void sbCreateSocket();
    void sbConnectToHost(const QString &hostName, quint16 port);
    void sbDisConnectFromHost();
    void sbWriteData(const QByteArray & byteArray);
signals:
    void saReadyRead();
    void saDisConnected();
    void saConnected();
    void sdReadedData(QByteArray & byteArray);

public slots:
    void sCreateSocket();
    void sConnectToHost(const QString &hostName, quint16 port);
    void sConnectToAddr(const QHostAddress & address, quint16 port);

    void sDisConnectFromHost();
    void sWriteData(const QByteArray & byteArray);
    void sReadData();
protected:
    int connectTimeout;
};

#endif // SOCKETHANDLER_H
