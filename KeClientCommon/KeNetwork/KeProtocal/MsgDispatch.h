#ifndef MSGDISPATCH_H
#define MSGDISPATCH_H

#include <QObject>
#include <QThread>
#include <QTcpSocket>

class Device;
class MsgDispatch : public QObject
{
    Q_OBJECT
public:
    explicit MsgDispatch(Device *dev);

    static QThread  msgThread;
    QAbstractSocket * msgSocket;
signals:
    void testSignal();
public slots:
    void GetMessageData();
    void stopGetMsg();
protected:
    Device * dispatchDev;
    QVector<char> msgRecv;
    int bufPos;
    char headBuf[11];
    int msgLen;
    int toRead;
    volatile  bool toStop;
    static const int msgMaxLen = 8192;
    // QThread interface

};

#endif // MSGDISPATCH_H
