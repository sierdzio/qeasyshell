#-------------------------------------------------
#
# Project created by QtCreator 2013-01-13T13:06:06
#
#-------------------------------------------------

include(../../global.pri)

QT       = core

TARGET   = redirection
CONFIG   += console
CONFIG   -= app_bundle
TEMPLATE = app
DESTDIR  = $${EXEC_DIRECTORY}

INCLUDEPATH += $${ROOT_DIRECTORY}/src
LIBS        += -L$${LIB_DIRECTORY} -lQEasyShell

SOURCES += \
    redirection.cpp
