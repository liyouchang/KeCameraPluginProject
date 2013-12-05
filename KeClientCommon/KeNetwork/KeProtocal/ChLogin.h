#ifndef CHLOGIN_H
#define CHLOGIN_H

#include <QObject>
#include "Channel.h"
#include "kenet_global.h"
class ChLogin : public Channel
{
    Q_OBJECT
public:
    explicit ChLogin(Channel *parent);
    QByteArray loginMD5;
    QByteArray loginName;
    QByteArray loginPwd;
    NET_DEVICEINFO devInfo;
    int result;
    void makeMd5(QString loginName,QString loginPWD,QByteArray key);

signals:

public slots:
    void OnRespond(QByteArray &msgData);

public:
    int Request();
};

#endif // CHLOGIN_H
