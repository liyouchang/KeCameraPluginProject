#include "Device.h"
#include "KeProtocal/ProtocalProcess.h"

Device::Device()
{
}

Device::Device(SocketHandler *s, ProtocalProcess *protocal,Device * parent) :
    Channel(s,protocal,parent)
{

}

Device::Device(SocketHandler *s, Device *parent):
    Channel(s,parent)
{
}

Device::Device(Device *parent):
     Channel(parent)
{
}

Device::~Device()
{
    qDeleteAll(childrenChannel);
//    for(int i=0;i<childrenChannel.size();++i){
//        childrenChannel[i]->deleteLater();
//    }
}

void Device::GetMessageData(QByteArray &allBytes)
{
    this->m_protocal->ExtractMessage(allBytes,this);
}

bool Device::AddChannel(Channel *c)
{
    if(childrenChannel.contains(c)){
        qWarning("Device::AddChannel allready have channel!");
        return false;
    }
    childrenChannel.append(c);
    c->m_parentDev = this;
    return true;
}

bool Device::RemoveChannel(Channel *c)
{

    return childrenChannel.removeOne(c);
}
