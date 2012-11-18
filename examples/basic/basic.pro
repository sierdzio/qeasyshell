#-------------------------------------------------
#
# Project created by QtCreator 2012-10-20T19:04:06
#
#-------------------------------------------------

include(../../global.pri)

QT       = core

TARGET   = basic
CONFIG   += console
CONFIG   -= app_bundle
TEMPLATE = app
DESTDIR  = $${EXEC_DIRECTORY}

INCLUDEPATH += $${ROOT_DIRECTORY}/src
LIBS        += -L$${LIB_DIRECTORY} -lQEasyShell

SOURCES += \
    basic.cpp

