#ifndef TESTRUNNER_H
#define TESTRUNNER_H

#include <QApplication>

#include <QObject>

#include <QList>
#include <QTimer>
#include <QCoreApplication>
#include <QtTest>
#include <QDialog>
class TestRunner : public QObject
{
    Q_OBJECT
public:
    explicit TestRunner(QObject *parent = 0);

    void addTest(QObject * test);
    bool runTests();
signals:

public slots:
    void run();
private:
    void doRunTests();
    QList<QObject *> m_tests;
    int m_overallResult;

};

#endif // TESTRUNNER_H
