
QT += network

INCLUDEPATH     += $$PWD
DEPENDPATH      += $$PWD

HEADERS += \
    $$PWD/KeProtocal/KeMessage.h \
    $$PWD/Controller/InstanceController.h \
    $$PWD/Controller/ConnectionController.h \
    $$PWD/HandleManager.h \
    $$PWD/kenet_global.h \
    $$PWD/Controller/RealPlayController.h \
    $$PWD/Controller/AbstractController.h \
    $$PWD/KeProtocal/ChRealData.h \
    $$PWD/Channel.h \
    $$PWD/Device.h \
    $$PWD/CommunicationThread.h \
    $$PWD/SocketHandler.h \
    $$PWD/Controller/RecordQueryController.h \
    $$PWD/KeProtocal/DevRecordQuery.h \
    $$PWD/Controller/AlarmController.h \
    $$PWD/KeProtocal/ProtocalProcess.h \
    $$PWD/KeProtocal/DevRealPlay.h \
    $$PWD/TcpListener.h \
    $$PWD/TcpDeviceHolder.h \
    $$PWD/DevConnection.h \
    $$PWD/Simulator/SimulatorController.h \
    $$PWD/Simulator/DevSimulateIPC.h \
    $$PWD/Simulator/ChSimulateVideo.h \
    $$PWD/Simulator/ProtocalIPC.h \
    $$PWD/KeProtocal/ChSecretKey.h \
    $$PWD/Simulator/ChDeviceKey.h \
    $$PWD/Simulator/ChDeviceLogin.h \
    $$PWD/Simulator/DevVideoSvr.h \
    $$PWD/Simulator/ChVideoFileReader.h \
    $$PWD/KeProtocal/ChPTZ.h \
    $$PWD/Controller/RecordDownloadController.h \
    $$PWD/KeProtocal/DevRecordDownload.h \
    $$PWD/KeProtocal/DevAlarm.h \
    $$PWD/KeProtocal/ChSearchDevice.h \
    $$PWD/KeProtocal/ChAskVideo.h \
    $$PWD/KeProtocal/ChLogin.h \
    $$PWD/KeProtocal/ProtocalKaerServer.h \
    $$PWD/KeProxy/ProtocalKaerProxy.h \
    $$PWD/KeProxy/DevConnectSvr.h \
    $$PWD/KeProxy/ChReceiveLogin.h \
    $$PWD/Controller/DeviceFactory.h

SOURCES += \
    $$PWD/Controller/InstanceController.cpp \
    $$PWD/Controller/ConnectionController.cpp \
    $$PWD/HandleManager.cpp \
    $$PWD/Controller/RealPlayController.cpp \
    $$PWD/Controller/AbstractController.cpp \
    $$PWD/KeProtocal/ChRealData.cpp \
    $$PWD/Channel.cpp \
    $$PWD/Device.cpp \
    $$PWD/CommunicationThread.cpp \
    $$PWD/SocketHandler.cpp \
    $$PWD/Controller/RecordQueryController.cpp \
    $$PWD/KeProtocal/DevRecordQuery.cpp \
    $$PWD/Controller/AlarmController.cpp \
    $$PWD/KeProtocal/ProtocalProcess.cpp \
    $$PWD/KeProtocal/DevRealPlay.cpp \
    $$PWD/TcpListener.cpp \
    $$PWD/TcpDeviceHolder.cpp \
    $$PWD/DevConnection.cpp \
    $$PWD/Simulator/SimulatorController.cpp \
    $$PWD/Simulator/DevSimulateIPC.cpp \
    $$PWD/Simulator/ChSimulateVideo.cpp \
    $$PWD/Simulator/ProtocalIPC.cpp \
    $$PWD/KeProtocal/ChSecretKey.cpp \
    $$PWD/Simulator/ChDeviceKey.cpp \
    $$PWD/Simulator/ChDeviceLogin.cpp \
    $$PWD/Simulator/DevVideoSvr.cpp \
    $$PWD/Simulator/ChVideoFileReader.cpp \
    $$PWD/KeProtocal/ChPTZ.cpp \
    $$PWD/Controller/RecordDownloadController.cpp \
    $$PWD/KeProtocal/DevRecordDownload.cpp \
    $$PWD/KeProtocal/DevAlarm.cpp \
    $$PWD/KeProtocal/ChSearchDevice.cpp \
    $$PWD/KeProtocal/ChAskVideo.cpp \
    $$PWD/KeProtocal/ChLogin.cpp \
    $$PWD/KeProtocal/ProtocalKaerServer.cpp \
    $$PWD/KeProxy/ProtocalKaerProxy.cpp \
    $$PWD/KeProxy/DevConnectSvr.cpp \
    $$PWD/KeProxy/ChReceiveLogin.cpp \
    $$PWD/Controller/DeviceFactory.cpp
