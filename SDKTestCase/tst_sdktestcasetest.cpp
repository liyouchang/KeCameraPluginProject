#include <QString>
#include <QtTest>
#include <QCoreApplication>

class SDKTestCaseTest : public QObject
{
    Q_OBJECT

public:
    SDKTestCaseTest();

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void testCase1();
    void testCase1_data();
};

SDKTestCaseTest::SDKTestCaseTest()
{
}

void SDKTestCaseTest::initTestCase()
{
}

void SDKTestCaseTest::cleanupTestCase()
{
}

void SDKTestCaseTest::testCase1()
{
    QFETCH(QString, data);
    QVERIFY2(true, "Failure");
}

void SDKTestCaseTest::testCase1_data()
{
    QTest::addColumn<QString>("data");
    QTest::newRow("0") << QString();
}

QTEST_MAIN(SDKTestCaseTest)

#include "tst_sdktestcasetest.moc"
