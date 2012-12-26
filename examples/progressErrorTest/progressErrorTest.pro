#-------------------------------------------------
#
# Project created by QtCreator 2012-12-26T12:20:07
#
#-------------------------------------------------

include(../../global.pri)

QT       = core

TARGET   = progressErrorTest
CONFIG   += console
CONFIG   -= app_bundle
TEMPLATE = app
DESTDIR  = $${EXEC_DIRECTORY}

INCLUDEPATH += $${ROOT_DIRECTORY}/src
LIBS        += -L$${LIB_DIRECTORY} -lQEasyShell

SOURCES += \
    progressErrorTest.cpp
