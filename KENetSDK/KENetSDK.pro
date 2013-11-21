#-------------------------------------------------
#
# Project created by QtCreator 2013-10-14T16:12:48
#
#-------------------------------------------------

QT       -= gui

TARGET = KENetSDK
TEMPLATE = lib

DEFINES += KENETSDK_LIBRARY

SOURCES += kenetsdk.cpp

HEADERS += kenetsdk.h

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}

include(../KeClientCommon/KeNetwork/KeNetwork.pri)
