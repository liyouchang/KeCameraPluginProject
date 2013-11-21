#-------------------------------------------------
#
# Project created by QtCreator 2013-09-28T16:18:38
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CameraClient
TEMPLATE = app

Debug:DESTDIR = $$PWD/../Debug
Release:DESTDIR = $$PWD/../Release

SOURCES += main.cpp\
        dialog.cpp \


HEADERS  += dialog.h \


include(../KeClientCommon/keClientCommon.pri)

