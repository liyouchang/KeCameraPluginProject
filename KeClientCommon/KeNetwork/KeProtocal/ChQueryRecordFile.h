#ifndef CHQUERYRECORDFILE_H
#define CHQUERYRECORDFILE_H

#include "Channel.h"
class ChQueryRecordFile : public Channel
{
    Q_OBJECT
public:
    explicit ChQueryRecordFile(int channelID, SocketHandler * s ,int type,QObject *parent);

signals:

public slots:


    // Channel interface
public:
    void OnRespond(const char *buf, int len);
    int Request();
};

#endif // CHQUERYRECORDFILE_H
