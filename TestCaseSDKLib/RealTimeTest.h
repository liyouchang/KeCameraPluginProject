#ifndef REALTIMETEST_H
#define REALTIMETEST_H

#include <QObject>
#include <QtTest>
#include "Controller/ConnectionController.h"
#include "Controller/InstanceController.h"

class RealTimeTest : public QObject
{
    Q_OBJECT
public:
    explicit RealTimeTest(QObject *parent = 0);
    ConnectionController * connection;
    InstanceController * inst;
signals:

public slots:
private slots:
    void initTestCase();
    void cleanupTestCase();
    void testCaseVideo();

};

#endif // REALTIMETEST_H
