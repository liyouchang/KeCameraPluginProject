#-------------------------------------------------
#
# Project created by QtCreator 2013-12-05T16:12:51
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = UpdateServer
TEMPLATE = app


SOURCES += main.cpp\
        UpdateServerWindow.cpp \
    UpdateDevModel.cpp \
    DeviceItem.cpp \
    UpdateServer.cpp \
    DevUpdate.cpp \
    UpdateProtocal.cpp

HEADERS  += UpdateServerWindow.h \
    UpdateDevModel.h \
    DeviceItem.h \
    UpdateServer.h \
    DevUpdate.h \
    KeUpdateMessage.h \
    UpdateProtocal.h

FORMS    += UpdateServerWindow.ui

Debug:DESTDIR = $$PWD/../Debug
Release:DESTDIR = $$PWD/../Release

include(../KeClientCommon/KeNetwork/KeNetwork.pri)
