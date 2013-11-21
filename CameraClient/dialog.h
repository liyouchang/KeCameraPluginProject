#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include "VideoWall.h"
#include "Controller/ConnectionController.h"
#include "Controller/RealPlayController.h"
#include "Controller/RecordQueryController.h"
#include "Controller/InstanceController.h"
class Dialog : public QDialog
{
    Q_OBJECT

public:
    Dialog(QWidget *parent = 0);
    ~Dialog();
    VideoWall * videoWall;
    QPushButton * setSplitBtn;
    QSpinBox *spinBox;
    ConnectionController * c;
    RealPlayController * r;
public slots:
    void doRealPlay();
    void doConnectDevice();
    void doQueryRecord();
    void setSplitclicked();
    void fullScreenBtnClicked();
    void testRealPlay();
    // QWidget interface
protected:
    void resizeEvent(QResizeEvent *);

};

#endif // DIALOG_H
