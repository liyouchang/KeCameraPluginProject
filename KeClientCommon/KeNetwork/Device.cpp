#include "Device.h"
Device::Device(SocketHandler *s, ProtocalProcess *protocal, QObject *parent) :
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


}

bool Device::AddChannel(Channel *c)
{
     return true;
}


//Channel *Device::GetChannel(int type, QString &name)
//{
//    Channel * find = 0;
//    QList<Channel *> allChildren = this->findChildren<Channel *>(name);
//    for(int i=0;i<allChildren.count();i++){
//        if(qobject_cast<Device *>(allChildren.at(i) )){
//            continue;
//        }
//        if(allChildren.at(i)->getType() == type){
//            find = allChildren.at(i);
//            break;
//        }
//    }
//    return find;
//}

