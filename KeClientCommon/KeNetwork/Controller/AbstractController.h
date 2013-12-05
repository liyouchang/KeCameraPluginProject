#ifndef ABSTRACTCONTROLLER_H
#define ABSTRACTCONTROLLER_H

#include "kenet_global.h"

class AbstractController
{
public:
    int getHandler();
    int getType();
    void setType(int type);
    virtual bool CheckAvaliable(int channelID);
    virtual void SetNetParam(NET_PARAM param);
    virtual NET_PARAM GetNetParam();

protected:

    void setHandler(int handle);
    AbstractController();
    virtual ~AbstractController();
    int controlHandler;
    int type;
    NET_PARAM netParam;

};



#endif // ABSTRACTCONTROLLER_H
