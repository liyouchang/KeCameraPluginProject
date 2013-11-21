#ifndef CONNECTIONCONTROLLER_H
#define CONNECTIONCONTROLLER_H

#include<string>
#include "kenet_global.h"
//using namespace std;
#include "AbstractController.h"

// 网络连接断开回调函数原形
typedef void (CALLBACK *fDisConnect)(int sID, char *addr, int port, void * user);
// 网络连接恢复回调函数原形
typedef void (CALLBACK *fHaveReConnect)(int sID, char *addr, int port, void * user);

class SocketHandler;
class ConnectionController:public AbstractController
{
public:
    virtual int ConnectDevice(const std::string &addr,int port) = 0;
    virtual int DisConnect() =0 ;
    virtual int LoginServer(std::string username,std::string pwd) =0;
    virtual int Logout() =0;
    virtual void SetDisConnectCallBack(fDisConnect cbDisConnect, void * user);
    virtual void SetAutoReconnectCallBack(fHaveReConnect cbAutoConnect,void *user);
    static ConnectionController *CreateInstance(AbstractController * parent);
    static ConnectionController *CreateInstance(SocketHandler * socket,AbstractController * parent);

protected:
    fDisConnect cbDisConnect;
    void * userDisConnect;
    fHaveReConnect cbReconnect;
    void * userReconnect;
};

#endif // CONNECTIONCONTROLLER_H
