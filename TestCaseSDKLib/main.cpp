#include <QCoreApplication>
#include "TestRunner.h"
#include <QDebug>

#include "RealTimeTest.h"
#include "ConnectionTest.h"
#include "RecordQueryTest.h"
int main(int argc, char *argv[])
{

    TestRunner testRunner;
    //testRunner.addTest(new ConnectionTest());
    //testRunner.addTest(new RealTimeTest());
    testRunner.addTest(new RecordQueryTest());
    qDebug() << "Overall result: " << (testRunner.runTests()?"PASS":"FAIL");

    return 0;
}
