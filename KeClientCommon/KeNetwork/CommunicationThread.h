#ifndef COMMUNICATIONTHREAD_H
#define COMMUNICATIONTHREAD_H

#include <QThread>
#include <QAbstractSocket>

class CommunicationThread : public QThread
{
    Q_OBJECT
public:
    explicit CommunicationThread(QObject *parent = 0);
    //QAbstractSocket * CreateSocket(int type);
    static CommunicationThread &instance();
};

class ChannelTread:public QThread
{
    Q_OBJECT
public:
    ChannelTread (QObject * parent = 0);
    static ChannelTread & instance();
};

#endif // COMMUNICATIONTHREAD_H
