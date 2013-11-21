#ifndef VIDEOFILEREADER_H
#define VIDEOFILEREADER_H

#include <QThread>
#include "Controller/ConnectionController.h"
#include "Simulator/SimulatorController.h"
enum ErrorMsgType;

class VideoFileReader : public QThread
{
    Q_OBJECT
public:
    explicit VideoFileReader(QObject *parent = 0);
    bool toStop;
    SimulatorController * ipc;
signals:
    void errorMessage(const QString &str, int error);
    void sendFileData(int channelNo,const char * data,int len);
public slots:
    //TODO:add Simulater Controller
    void NewSimulater(ConnectionController * parent);
    //TODO:read file and send video data
    void fileReader();
protected:



    // QThread interface
protected:
    void run();
};

#endif // VIDEOFILEREADER_H
