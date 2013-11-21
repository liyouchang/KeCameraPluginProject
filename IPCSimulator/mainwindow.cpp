#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "VideoFileReader.h"
#include <QTime>
#include <QDataStream>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    inst = InstanceController::CreateInstance(KE_IPC_Simulator);
    simulator = SimulatorController::CreateInstance(inst);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_startButton_clicked()
{
    int ret = simulator->ConnectDevice("192.168.0.8",22616);
    if(ret != 0){
        this->ErrorMessage("connect server error",EMsg_Error);
    }
    simulator->SetMac("008f4bd29941");
    ret = simulator->LoginServer("system","123456");
    if(ret != 0){
        this->ErrorMessage(QString("connect server error %1").arg(ret),EMsg_Error);
    }
    VideoFileReader * reader = new VideoFileReader(this);
    reader->ipc = simulator;
    QObject::connect(reader,&VideoFileReader::errorMessage,this,&MainWindow::ErrorMessage);
    reader->start();
}

void MainWindow::on_testButton_clicked()
{

    VideoFileReader * reader = new VideoFileReader(this);
    QObject::connect(reader,&VideoFileReader::errorMessage,this,&MainWindow::ErrorMessage);
    reader->start();
}

void stringToHtmlFilter(QString &str)
{
    //注意这几行代码的顺序不能乱，否则会造成多次替换
    str.replace("&","&amp;");
    str.replace(">","&gt;");
    str.replace("<","&lt;");
    str.replace("\"","&quot;");
    str.replace("\'","&#39;");
    str.replace(" ","&nbsp;");
    str.replace("\n","<br>");
    str.replace("\r","<br>");
}

void stringToHtml(QString &str,QColor crl)
{
    QByteArray array;
    array.append(crl.red());
    array.append(crl.green());
    array.append(crl.blue());
    QString strC(array.toHex());

    str =  QString("<span  style='color:#%1;'>%2</span>").arg(strC).arg(str);
}
void MainWindow::ErrorMessage(const QString &str, int error)
{
    QString htmlStr =  QTime::currentTime().toString() + " : " + str;
    stringToHtmlFilter(htmlStr);
    switch (error){
    case EMsg_Info:
        stringToHtml(htmlStr,QColor(Qt::black));
        break;
    case EMsg_Warning:
        stringToHtml(htmlStr,QColor(Qt::darkMagenta	));
        break;
    case EMsg_Error:
        stringToHtml(htmlStr,QColor(Qt::red));
        break;
    }
    ui->textEdit->insertHtml(htmlStr);
    ui->textEdit->insertHtml("<br>");

}
