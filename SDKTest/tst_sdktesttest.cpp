#include <QString>
#include <QtTest>

class SDKTestTest : public QObject
{
    Q_OBJECT

public:
    SDKTestTest();

private Q_SLOTS:
    void testCase1();
};

SDKTestTest::SDKTestTest()
{
}

void SDKTestTest::testCase1()
{
    QVERIFY2(false, "Failure");
}

QTEST_MAIN(SDKTestTest)

#include "tst_sdktesttest.moc"
