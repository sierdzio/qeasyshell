#-------------------------------------------------
#
# Project created by QtCreator 2012-10-20T16:13:25
#
#-------------------------------------------------

include(../global.pri)

TARGET = QEasyShell
TEMPLATE = lib
VERSION = 1.0.0

DEFINES += QEASYSHELL_LIBRARY
DEFINES += DEBUG

SOURCES += \
           qescommand.cpp \
           qesresult.cpp \
           qessubcommand.cpp \
           qesprocess.cpp

HEADERS += \
           qeasyshellglobal.h \
           qescommand.h \
           qesdefinitions.h \
           qesresult.h \
           qessubcommand.h \
           qesprocess.h
