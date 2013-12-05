#include "ProtocalKaerProxy.h"
#include "KeProtocal/KeMessage.h"
#include "KeProtocal/DevRealPlay.h"
#include "KeProtocal/ChPTZ.h"
#include "KeProtocal/DevRecordDownload.h"
#include "KeProtocal/DevAlarm.h"
#include "KeProtocal/ChAskVideo.h"
#include "KeProtocal/ChLogin.h"
ProtocalKaerProxy::ProtocalKaerProxy(QObject *parent) :
    ProtocalProcess(parent)
{
}

