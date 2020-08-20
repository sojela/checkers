QT += testlib
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

INCDIR = ../app/src
INCLUDEPATH += $$INCDIR
HEADERS += $$INCDIR/checkerslogic.h
SOURCES += $$INCDIR/checkerslogic.cpp

SOURCES +=  tst_checkerslogic.cpp