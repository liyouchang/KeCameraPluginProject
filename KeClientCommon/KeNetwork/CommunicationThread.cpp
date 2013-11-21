#include "CommunicationThread.h"
#include <QTcpSocket>
CommunicationThread::CommunicationThread(QObject *parent) :
    QThread(parent)
{
}

CommunicationThread &CommunicationThread::instance()
{
    static CommunicationThread manager;
    return manager;
}



ChannelTread::ChannelTread(QObject *parent):
    QThread(parent)
{

}

ChannelTread &ChannelTread::instance()
{
    static ChannelTread signal;
    return signal;

}
