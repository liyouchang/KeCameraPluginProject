#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Controller/InstanceController.h"
#include "Simulator/SimulatorController.h"
#include <QString>
#include <QColor>
namespace Ui {
class MainWindow;
}

enum ErrorMsgType{
    EMsg_Info,
    EMsg_Warning,
    EMsg_Error
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
signals:
    void showErrorText(const QString & text);
private slots:
    void on_startButton_clicked();
    void on_testButton_clicked();
public slots:
    void ErrorMessage(const QString &str,int error);
public:
    InstanceController * inst;
    SimulatorController * simulator;
private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
