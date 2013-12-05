#ifndef CONNECTIONTEST_H
#define CONNECTIONTEST_H

#include <QObject>
#include <QtTest>
#include "Controller/InstanceController.h"

class ConnectionTest : public QObject
{
    Q_OBJECT
public:
    explicit ConnectionTest(QObject *parent = 0);
    InstanceController * inst;
    QString ip;
    int port;
signals:

public slots:
private slots:
    void initTestCase();
    void cleanupTestCase();

    void testNormalConnection();
    void testConnectionError();
    void testLogin();
    void testTimeParam();
    void testTryConnect();
};

#endif // CONNECTIONTEST_H
