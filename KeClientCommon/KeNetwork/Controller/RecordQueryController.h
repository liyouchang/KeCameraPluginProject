#ifndef RECORDQUERYCONTROLLER_H
#define RECORDQUERYCONTROLLER_H

#include "AbstractController.h"
#include "kenet_global.h"
#include <vector>
class RecordQueryController:public AbstractController
{
public:
    RecordQueryController();
    virtual void SetTimeout(int time) = 0;
    virtual void SetQueryTime(NET_TIME  tmStart, NET_TIME  tmEnd) = 0;
    virtual void SetQueryType(int recordFileType) = 0;
    virtual int QueryRecordFile(int channelID,std::vector<NET_RECORDFILE_INFO> & vFileInfo) = 0;
    virtual int QuickQueryRecordFile(int channelID,std::vector<NET_RECORDFILE_INFO> & vFileInfo) = 0;
    virtual int FindStart(int channelID) = 0;
    virtual int FindNext(NET_RECORDFILE_INFO * lpFindData) = 0;
    virtual int FindClose() = 0;
    static RecordQueryController *CreateInstance(AbstractController * parent);

};

#endif // RECORDQUERYCONTROLLER_H
