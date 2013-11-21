
QT += network

INCLUDEPATH     += $$PWD
DEPENDPATH      += $$PWD

HEADERS += \
    ../KeClientCommon/KeNetwork/KeProtocal/KeMessage.h \
    ../KeClientCommon/KeNetwork/KeProtocal/KeTalkChannel.h \
    ../KeClientCommon/KeNetwork/Controller/InstanceController.h \
    ../KeClientCommon/KeNetwork/Controller/ConnectionController.h \
    ../KeClientCommon/KeNetwork/HandleManager.h \
    ../KeClientCommon/KeNetwork/kenet_global.h \
    ../KeClientCommon/KeNetwork/Controller/RealPlayController.h \
    ../KeClientCommon/KeNetwork/Controller/AbstractController.h \
    ../KeClientCommon/KeNetwork/KeProtocal/ChRealData.h \
    ../KeClientCommon/KeNetwork/Channel.h \
    ../KeClientCommon/KeNetwork/Device.h \
    ../KeClientCommon/KeNetwork/CommunicationThread.h \
    ../KeClientCommon/KeNetwork/SocketHandler.h \
    ../KeClientCommon/KeNetwork/Controller/RecordQueryController.h \
    ../KeClientCommon/KeNetwork/KeProtocal/DevRecordQuery.h \
    ../KeClientCommon/KeNetwork/Controller/AlarmController.h \
    ../KeClientCommon/KeNetwork/KeProtocal/ProtocalProcess.h \
    ../KeClientCommon/KeNetwork/KeProtocal/DevRealPlay.h \
    ../KeClientCommon/KeNetwork/TcpListener.h \
    ../KeClientCommon/KeNetwork/TcpDeviceHolder.h \
    ../KeClientCommon/KeNetwork/DevConnection.h \
    ../KeClientCommon/KeNetwork/Simulator/SimulatorController.h \
    ../KeClientCommon/KeNetwork/Simulator/DevSimulateIPC.h \
    ../KeClientCommon/KeNetwork/Simulator/ChSimulateVideo.h \
    ../KeClientCommon/KeNetwork/Simulator/ProtocalIPC.h \
    ../KeClientCommon/KeNetwork/KeProtocal/ChSecretKey.h \
    ../KeClientCommon/KeNetwork/Simulator/ChDeviceKey.h \
    ../KeClientCommon/KeNetwork/Simulator/ChDeviceLogin.h \
    ../KeClientCommon/KeNetwork/Simulator/DevVideoSvr.h \
    ../KeClientCommon/KeNetwork/Simulator/ChVideoFileReader.h \
    ../KeClientCommon/KeNetwork/KeProtocal/ChPTZ.h \
    ../KeClientCommon/KeNetwork/Controller/RecordDownloadController.h \
    ../KeClientCommon/KeNetwork/KeProtocal/DevRecordDownload.h \
    ../KeClientCommon/KeNetwork/KeProtocal/DevAlarm.h

SOURCES += \
    ../KeClientCommon/KeNetwork/KeProtocal/KeTalkChannel.cpp \
    ../KeClientCommon/KeNetwork/Controller/InstanceController.cpp \
    ../KeClientCommon/KeNetwork/Controller/ConnectionController.cpp \
    ../KeClientCommon/KeNetwork/HandleManager.cpp \
    ../KeClientCommon/KeNetwork/Controller/RealPlayController.cpp \
    ../KeClientCommon/KeNetwork/Controller/AbstractController.cpp \
    ../KeClientCommon/KeNetwork/KeProtocal/ChRealData.cpp \
    ../KeClientCommon/KeNetwork/Channel.cpp \
    ../KeClientCommon/KeNetwork/Device.cpp \
    ../KeClientCommon/KeNetwork/CommunicationThread.cpp \
    ../KeClientCommon/KeNetwork/SocketHandler.cpp \
    ../KeClientCommon/KeNetwork/Controller/RecordQueryController.cpp \
    ../KeClientCommon/KeNetwork/KeProtocal/DevRecordQuery.cpp \
    ../KeClientCommon/KeNetwork/Controller/AlarmController.cpp \
    ../KeClientCommon/KeNetwork/KeProtocal/ProtocalProcess.cpp \
    ../KeClientCommon/KeNetwork/KeProtocal/DevRealPlay.cpp \
    ../KeClientCommon/KeNetwork/TcpListener.cpp \
    ../KeClientCommon/KeNetwork/TcpDeviceHolder.cpp \
    ../KeClientCommon/KeNetwork/DevConnection.cpp \
    ../KeClientCommon/KeNetwork/Simulator/SimulatorController.cpp \
    ../KeClientCommon/KeNetwork/Simulator/DevSimulateIPC.cpp \
    ../KeClientCommon/KeNetwork/Simulator/ChSimulateVideo.cpp \
    ../KeClientCommon/KeNetwork/Simulator/ProtocalIPC.cpp \
    ../KeClientCommon/KeNetwork/KeProtocal/ChSecretKey.cpp \
    ../KeClientCommon/KeNetwork/Simulator/ChDeviceKey.cpp \
    ../KeClientCommon/KeNetwork/Simulator/ChDeviceLogin.cpp \
    ../KeClientCommon/KeNetwork/Simulator/DevVideoSvr.cpp \
    ../KeClientCommon/KeNetwork/Simulator/ChVideoFileReader.cpp \
    ../KeClientCommon/KeNetwork/KeProtocal/ChPTZ.cpp \
    ../KeClientCommon/KeNetwork/Controller/RecordDownloadController.cpp \
    ../KeClientCommon/KeNetwork/KeProtocal/DevRecordDownload.cpp \
    ../KeClientCommon/KeNetwork/KeProtocal/DevAlarm.cpp
