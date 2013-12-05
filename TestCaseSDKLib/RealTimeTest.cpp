#include "RealTimeTest.h"
#include "Controller/AbstractController.h"
#include "Controller/ConnectionController.h"
#include "Controller/RealPlayController.h"
#include "VideoWall.h"
#include <QDialog>
#include <QThread>
RealTimeTest::RealTimeTest(QObject *parent) :
    QObject(parent)
{
}

void RealTimeTest::initTestCase()
{
    inst = InstanceController::CreateInstance(KE_DevType_DVR);
    QVERIFY2(inst!=0,"instance create  error");

    connection  = ConnectionController::CreateInstance(inst);
    QVERIFY2(connection!=0,"connection create  error");

    int ret = connection->ConnectDevice("192.168.0.212",22616);
    QVERIFY2(ret==KE_SUCCESS,"connect error");

    ret = connection->LoginServer("system","123456");
    QVERIFY2(ret==KE_SUCCESS,"login error");

}

void RealTimeTest::cleanupTestCase()
{
    delete inst;
}

void RealTimeTest::testCaseVideo()
{
    RealPlayController * real = RealPlayController::CreateInstance(this->connection);
    QVERIFY2(real!=0,"realplay create  error");

    int ret = real->StartRealPlay(1);
    QVERIFY2(ret == KE_SUCCESS,"Real play start error");

    ret = real->StartSound();
    QVERIFY2(ret == KE_SUCCESS,"sound start error");

    ret = real->StartTalk();
    QVERIFY2(ret == KE_SUCCESS,"talk start error");

    ret = real->StopSound();
    QVERIFY2(ret == KE_SUCCESS,"sound stop error");

    ret = real->StartTalk();
    QVERIFY2(ret == KE_SUCCESS,"talk stop error");

    ret = real->StopRealPlay();
    QVERIFY2(ret == KE_SUCCESS,"Real play stop error");

}

