#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "kenetsdk.h"
#include "IPCHandler.h"
namespace Ui {
class DemoMainWindow;
}



class DemoMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit DemoMainWindow(QWidget *parent = 0);
    ~DemoMainWindow();
signals:
    void sendErrorMessage(QString str,int error);
public slots:
    void ErrorMessage(const QString &str,int error);

private slots:
    void on_btnConnect_clicked();

    void on_btnSearchDev_clicked();

    void on_btnInitial_clicked();

    void on_btnStartView_clicked();

    void on_btnStopView_clicked();

    void on_btnPTZUp_pressed();

    void on_btnPTZUp_released();

    void on_btnPTZRight_released();

    void on_btnPTZDown_pressed();

    void on_btnPTZDown_released();

    void on_btnPTZLeft_pressed();

    void on_btnPTZLeft_released();

    void on_btnPTZRight_pressed();

    void on_btnStartQueryRecord_clicked();

    void on_btnNextRecord_clicked();

    void on_btnStopQuery_clicked();

    void on_btnSaveRecord_clicked();

    void on_btnStopSave_clicked();

    void on_btnStartAlarm_clicked();

    void on_btnStopAlarm_clicked();

    void on_btnDisconnect_clicked();

    void on_btnStartListen_clicked();

private:
    Ui::DemoMainWindow *ui;
public:
    IPCHandler sdkInst;
    NET_RECORDFILE_INFO record;
    int downloadPercent;
};

#endif // MAINWINDOW_H
