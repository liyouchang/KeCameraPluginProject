#include <QString>
#include <QtTest>

class TestQString: public QObject
{
    Q_OBJECT

public:
    TestQString(){}

private slots:
    void toUpper();
};



void TestQString::toUpper()
{
    QString str = "Hello";
    QVERIFY(str.toUpper()== "HELLO");
}

QTEST_MAIN(TestQString)

#include "TestQString.moc"

