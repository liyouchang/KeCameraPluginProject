#-------------------------------------------------
#
# Project created by QtCreator 2013-11-13T13:49:40
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = IPCSimulator
TEMPLATE = app


Debug:DESTDIR = $$PWD/../Debug
Release:DESTDIR = $$PWD/../Release


SOURCES += main.cpp\
        mainwindow.cpp \
    VideoFileReader.cpp

HEADERS  += mainwindow.h \
    VideoFileReader.h

FORMS    += mainwindow.ui
include(../KeClientCommon/KeNetwork/KeNetwork.pri)
)
