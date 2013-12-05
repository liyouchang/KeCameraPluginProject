#include "RecordQueryTest.h"
#include "Controller/RecordQueryController.h"
#include <vector>
#include <QDateTime>
RecordQueryTest::RecordQueryTest(QObject *parent) :
    QObject(parent)
{
}

void RecordQueryTest::initTestCase()
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

void RecordQueryTest::cleanupTestCase()
{
    delete inst;

}

void RecordQueryTest::testCaseQuery()
{
    RecordQueryController * real = RecordQueryController::CreateInstance(this->connection);
    QVERIFY2(real!=0,"RecordQuery create  error");

     //= QDateTime::currentDateTime();
    QDateTime endTime = QDateTime::currentDateTime();
    QDateTime startTime = endTime.addDays(-4);
    NET_TIME st;
    st.year = startTime.date().year();
    st.month = startTime.date().month();
    st.day = startTime.date().day();
    st.hour = startTime.time().hour();
    st.minute = startTime.time().minute();
    st.second = startTime.time().second();

    NET_TIME et;
    et.year = endTime.date().year();
    et.month = endTime.date().month();
    et.day = endTime.date().day();
    et.hour = endTime.time().hour();
    et.minute = endTime.time().minute();
    et.second = endTime.time().second();

    real->SetQueryType(KE_Terminal_All_Record);
    real->SetQueryTime(st,et);

    int ret = 0;
    std::vector<NET_RECORDFILE_INFO> vFileInfo;
    ret =  real->QueryRecordFile(1,vFileInfo);
    QVERIFY2(ret ==0,"QueryRecordFile  error");

    qDebug("record file size %d",vFileInfo.size());

    std::vector<NET_RECORDFILE_INFO> vQuickFileInfo;
    real->SetQueryTime(st,et);
    ret = real->QuickQueryRecordFile(1,vQuickFileInfo);
    QVERIFY2(ret ==0,"QuickQueryRecordFile  error");

    qDebug("record file size %d",vQuickFileInfo.size());
}
