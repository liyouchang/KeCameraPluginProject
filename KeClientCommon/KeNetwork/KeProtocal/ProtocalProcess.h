#ifndef PROTOCALPROCESS_H
#define PROTOCALPROCESS_H

#include <QObject>
#include "ChRealData.h"
#include "DevRecordQuery.h"
#include "DevConnection.h"
#include <QByteArray>


class DevRealPlay;
class ProtocalProcess : public QObject
{
    Q_OBJECT
public:
    explicit ProtocalProcess(QObject *parent = 0);

signals:

public slots:
    virtual void ExtractMessage(QByteArray & allBytes, Channel *parser);
public:
    virtual QByteArray CreateMessage(ChRealData *ch);
    virtual QByteArray CreateMessage(DevRealPlay *ch);


public:
    virtual void ParseMessage(QByteArray & msgData,DevConnection * ch);
    virtual void ParseMessage(QByteArray &msgData, DevRealPlay *ch);
    virtual void ParseMessage(QByteArray &msgData, ChRealData *ch);
    virtual void ParseMessage(QByteArray &msgData, DevRecordQuery *ch);



protected:
    QByteArray msgRecv;
    int bufPos;
    char headBuf[11];
    int toRead;
    static const int msgMaxLen = 8192;
};

#endif // PROTOCALPROCESS_H
