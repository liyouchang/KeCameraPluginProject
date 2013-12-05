#ifndef RECORDQUERYTEST_H
#define RECORDQUERYTEST_H

#include <QObject>
#include <QtTest>

#include "Controller/ConnectionController.h"
#include "Controller/InstanceController.h"

class RecordQueryTest : public QObject
{
    Q_OBJECT
public:
    explicit RecordQueryTest(QObject *parent = 0);
    ConnectionController * connection;
    InstanceController * inst;

signals:

public slots:
private slots:
    void initTestCase();
    void cleanupTestCase();
    void testCaseQuery();

};

#endif // RECORDQUERYTEST_H
