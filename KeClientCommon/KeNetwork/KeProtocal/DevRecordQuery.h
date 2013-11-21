#ifndef DEVRECORDQUERY_H
#define DEVRECORDQUERY_H

#include "Device.h"
#include "Controller/RecordQueryController.h"
class DevRecordQuery : public Device,public RecordQueryController
{
    Q_OBJECT
public:
    explicit DevRecordQuery(Device *parent);
    std::vector<NET_RECORDFILE_INFO> recordFileList;
    int m_recordFileType;
    NET_TIME m_startTime;
    NET_TIME m_endTime;
    bool findEnd;
    int recordPos;
protected:
    int m_queryTimeout;

signals:

public slots:
    void OnRespond(QByteArray & data);
public:
    void SetTimeout(int time);
    void SetQueryTime(NET_TIME tmStart, NET_TIME tmEnd);
    void SetQueryType(int recordFileType);
    int QueryRecordFile(int channelID, std::vector<NET_RECORDFILE_INFO> &vFileInfo);
    int QuickQueryRecordFile(int channelID, std::vector<NET_RECORDFILE_INFO> &vFileInfo);
    int FindStart(int channelID);
    int FindNext(NET_RECORDFILE_INFO *lpFindData);
    int FindClose();
    bool CheckAvaliable(int channelID);

    int Request();
};

#endif // DEVRECORDQUERY_H
