#ifndef CHDEVICELOGIN_H
#define CHDEVICELOGIN_H
#include "Channel.h"
class ChDeviceLogin : public Channel
{
    Q_OBJECT
public:
    explicit ChDeviceLogin(Channel *parent);
    void initialize(QString loginName,QString loginPWD,QByteArray key);
signals:
    // Channel interface
public slots:
    void OnRespond(QByteArray &msgData);
public:
    int Request();
public:
    QByteArray loginMD5;
    QByteArray loginName;
    QByteArray loginPwd;
    int result;
};

#endif // CHDEVICELOGIN_H
