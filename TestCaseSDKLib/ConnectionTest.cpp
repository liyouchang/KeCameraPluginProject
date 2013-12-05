#include "ConnectionTest.h"
#include "Controller/ConnectionController.h"

ConnectionTest::ConnectionTest(QObject *parent) :
    QObject(parent)
{

}

void ConnectionTest::initTestCase()
{
    inst = InstanceController::CreateInstance(KE_DevType_DVR);
    QVERIFY2(inst!=0,"instance create  error");
}

void ConnectionTest::cleanupTestCase()
{
    delete inst;
    inst = 0;
}

void ConnectionTest::testNormalConnection()
{
        ConnectionController * connect = ConnectionController::CreateInstance(this->inst);
        QVERIFY2(connect!=0,"connect create error");
        int ret = connect->ConnectDevice("192.168.0.212",22616);
        QVERIFY2(ret == KE_SUCCESS,"connect error");
        ret = connect->DisConnect();
        QVERIFY2(ret == KE_SUCCESS,"DisConnect error");

        //reconnect
        ret = connect->ConnectDevice("192.168.0.212",22616);
        QVERIFY2(ret == KE_SUCCESS,"reconnect error");
        ret = connect->DisConnect();
        QVERIFY2(ret == KE_SUCCESS,"ReDisConnect error");
        delete connect;

}

void ConnectionTest::testConnectionError()
{
    ConnectionController * connect = ConnectionController::CreateInstance(this->inst);
    QVERIFY2(connect!=0,"connect create error");
    int ret = 0;
    ret = connect->ConnectDevice("192.168.0.210",22616);
    QVERIFY2(ret == KE_NETCONNECT_ERROR,"connect wrong ip");
    ret = connect->DisConnect();
    QVERIFY2(ret == KE_SUCCESS,"DisConnect error");

    ret = connect->ConnectDevice("192.168.0.212",22617);
    QVERIFY2(ret == KE_NETCONNECT_ERROR,"connect wrong port");
    ret = connect->DisConnect();
    QVERIFY2(ret == KE_SUCCESS,"error");

    delete connect;
}

void ConnectionTest::testLogin()
{
    ConnectionController * connect = ConnectionController::CreateInstance(this->inst);
    QVERIFY2(connect!=0,"connect create error");
    int ret = connect->ConnectDevice("192.168.0.212",22616);
    QVERIFY2(ret == KE_SUCCESS,"connect error");

    //error login
    ret = connect->LoginServer("system","456789");
    QVERIFY2(ret == KE_Login_Error,"login error");

    //success login
    ret = connect->LoginServer("system","123456");
    QVERIFY2(ret == KE_SUCCESS,"login error");

    //relogin,terminal return 0x0d is not right
    //ret = connect->LoginServer("system","123456");
    //QVERIFY2(ret == KE_Relogin,"should return KE_Relogin");

    delete connect;
}

void ConnectionTest::testTimeParam()
{
    NET_PARAM inParam;
    inParam.waitTimeout = 2000;
    ConnectionController * connect = ConnectionController::CreateInstance(this->inst);
    this->inst->SetNetParam(inParam);
    NET_PARAM outParam;
    outParam =  connect->GetNetParam();
    QVERIFY2(outParam.waitTimeout == inParam.waitTimeout,"timeout error");
    delete connect;

}

void ConnectionTest::testTryConnect()
{
    ConnectionController * connect = ConnectionController::CreateInstance(this->inst);
    QVERIFY2(connect!=0,"connect create error");
    connect->TryConnect("192.168.0.212",22616);
    //QThread::sleep(11);
}


