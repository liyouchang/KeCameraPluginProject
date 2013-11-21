#include "KeTalkChannel.h"

KeTalkChannel::KeTalkChannel(int cameraID, SocketHandler *s, QObject *parent) :
    Channel(s,0,parent),mCameraID(cameraID)
{

}



int KeTalkChannel::Request()
{
    return 0;
}

void KeTalkChannel::setTalkData(const char *buf, int len)
{
    this->talkData = QByteArray(buf,len);
}
