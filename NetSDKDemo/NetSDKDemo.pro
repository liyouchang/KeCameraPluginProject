#-------------------------------------------------
#
# Project created by QtCreator 2013-11-22T09:42:18
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = NetSDKDemo
TEMPLATE = app


SOURCES += \
    DemoMainWindow.cpp \
    SDKDemo.cpp \
    DlgSearchDevice.cpp \
    IPCHandler.cpp

HEADERS  += \
    kenetsdk.h \
    kenet_global.h \
    DemoMainWindow.h \
    DlgSearchDevice.h \
    IPCHandler.h

FORMS    += \
    DemonMainWindow.ui \
    DlgSearchDevice.ui



Debug:DESTDIR = $$PWD/../Debug
Release:DESTDIR = $$PWD/../Release

LIBS += -L$$DESTDIR -lKENetSDK

message("demo libs is"  $$LIBS)

include(../KeClientCommon/KePlayView/KePlayView.pri)
include(../KeClientCommon/MyControl/MyControl.pri)

OTHER_FILES +=
