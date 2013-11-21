#include "dialog.h"
#include <QTimer>
Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
{
    videoWall = new VideoWall(this);
    this->resize(QSize(600,500));
    QVBoxLayout * mainLayout = new QVBoxLayout;
    mainLayout->addWidget(videoWall);
    QHBoxLayout * controlLayout = new QHBoxLayout;
    spinBox = new QSpinBox(this);
    setSplitBtn = new QPushButton(this);
    setSplitBtn->setText("change split screen");
    QObject::connect(setSplitBtn,&QPushButton::clicked,this,&Dialog::setSplitclicked);
    QPushButton * fullScreenBtn = new QPushButton(this);
    fullScreenBtn->setText("full screen");
    QObject::connect(fullScreenBtn,&QPushButton::clicked,this,&Dialog::fullScreenBtnClicked);
    spinBox->setValue(4);

    QPushButton * startPlayTest = new QPushButton(this);
    startPlayTest->setText("start play");
    QObject::connect(startPlayTest,&QPushButton::clicked,this,&Dialog::doRealPlay);

    QPushButton * connectTest = new QPushButton(this);
    connectTest->setText("connect device");
    QObject::connect(connectTest,&QPushButton::clicked,this,&Dialog::doConnectDevice);

    QPushButton * recordTest = new QPushButton(this);
    recordTest->setText("query record");
    QObject::connect(recordTest,&QPushButton::clicked,this,&Dialog::doQueryRecord);

    controlLayout->addWidget(recordTest);
    controlLayout->addWidget(connectTest);
    controlLayout->addWidget(startPlayTest);
    controlLayout->addWidget(spinBox);
    controlLayout->addWidget(setSplitBtn);
    controlLayout->addWidget(fullScreenBtn);
    mainLayout->addLayout(controlLayout);
    this->setLayout(mainLayout);



}

Dialog::~Dialog()
{

}

void CALLBACK RealDataCallBack(int realHandle, int dataType,const char *buffer, int bufSize, void * user)
{
    VideoWall * v = static_cast<VideoWall *>(user);
    QByteArray data(buffer,bufSize);
    v->MediaDataRecv(0,dataType,data);
}

void Dialog::doRealPlay()
{
    qDebug("doRealPlay");

    //    r = RealPlayController::CreateInstance(KEDVR,c);
    //    r->StartRealPlay(1);
    //    //op = new NetworkOperator(this);
    //    r->SetRealDataCallBack(RealDataCallBack,this->videoWall);
    r = RealPlayController::CreateInstance(c);
    r->SetRealDataCallBack(RealDataCallBack,this->videoWall);
    r->StartRealPlay(1);
    //op = new NetworkOperator(this);

}

void Dialog::doConnectDevice()
{
    InstanceController * instance = InstanceController::CreateInstance(KE_DevType_DVR);
    c = ConnectionController::CreateInstance(instance);
    int ret = c->ConnectDevice("192.168.50.20",22616);
    if(ret != 0 ){
        qDebug("connect device error!");
    }
    qDebug("connect device success!");
}

void Dialog::doQueryRecord()
{
    RecordQueryController * r = RecordQueryController::CreateInstance(c);
    NET_TIME start;
    start.year = 2013;
    start.month = 11;
    start.day = 8;
    start.hour = 8;
    start.minute = 0;
    start.second = 0;
    NET_TIME end = start;
    end.hour = 18;
    r->SetQueryTime(start,end);
    r->SetQueryType(1);
    std::vector<NET_RECORDFILE_INFO> fileList;
    int ret = r->QueryRecordFile(1,fileList);
    qDebug("QueryRecordFile  ret = %d",ret);

}


void Dialog::setSplitclicked()
{
    int num =spinBox->value();
    videoWall->SetDivision(num);
}

void Dialog::fullScreenBtnClicked()
{
    // videoWall->setWindowState(videoWall->windowState() ^ Qt::WindowFullScreen);
    //    videoWall->setWindowFlags(Qt::Dialog);
    //    videoWall->showFullScreen();
    videoWall->showfullScreenWall();
}


void Dialog::testRealPlay()
{
    qDebug("start play");

    QTimer::singleShot(0,this,SLOT(doConnectDevice()));
    QTimer::singleShot(0,this,SLOT(doRealPlay()));
    //op->initial(0);
    //op->ConnectDevice("192.168.0.202",22616);
    //QObject::connect(c,&KeDvrDevice::DevConnect,videoWall,&VideoWall::MediaDataRecv);
    // op->StartRealPlay(1);
}

void Dialog::resizeEvent(QResizeEvent *)
{
    QRect r(rect());

    // this->videoWall->setGeometry(r.x(),r.y(),r.width(),r.height()-100);
}
