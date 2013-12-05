#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Controller/InstanceController.h"
#include "Simulator/SimulatorController.h"
#include <QString>
#include <QColor>
#include "VideoFileReader.h"

namespace Ui {
class MainWindow;
}


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
signals:
    void showErrorText(const QString & text);
private slots:
    void on_btnLogin_clicked();

    void on_btnVideo_clicked();

public slots:
    void ErrorMessage(const QString &str,int error);
public:
    InstanceController * inst;
    SimulatorController * simulator;
    VideoFileReader * reader;
private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
