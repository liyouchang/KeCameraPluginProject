#-------------------------------------------------
#
# Project created by QtCreator 2013-09-28T16:18:38
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CameraClient
TEMPLATE = app



SOURCES += main.cpp\
        dialog.cpp \
    TestDialog.cpp


HEADERS  += dialog.h \
    TestDialog.h

Debug:DESTDIR = $$PWD/../Debug
Release:DESTDIR = $$PWD/../Release
include(../KeClientCommon/keClientCommon.pri)

FORMS += \
    TestDialog.ui

