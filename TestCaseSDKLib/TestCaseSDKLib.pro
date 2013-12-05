#-------------------------------------------------
#
# Project created by QtCreator 2013-11-21T16:38:06
#
#-------------------------------------------------

QT       += core gui testlib
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets



TARGET = TestCaseSDKLib
#CONFIG   += console
#CONFIG   -= app_bundle

TEMPLATE = app


Debug:DESTDIR = $$PWD/../Debug
Release:DESTDIR = $$PWD/../Release

SOURCES += main.cpp \
    TestRunner.cpp \
    RealTimeTest.cpp \
    ConnectionTest.cpp \
    RecordQueryTest.cpp

HEADERS += \
    TestRunner.h \
    RealTimeTest.h \
    ConnectionTest.h \
    RecordQueryTest.h

include(../KeClientCommon/keClientCommon.pri)
