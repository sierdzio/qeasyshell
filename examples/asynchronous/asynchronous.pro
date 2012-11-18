#-------------------------------------------------
#
# Project created by QtCreator 2012-11-18T16:26:08
#
#-------------------------------------------------

include(../../global.pri)

QT       = core

TARGET   = asynchronous
CONFIG   += console
CONFIG   -= app_bundle
TEMPLATE = app
DESTDIR  = $${EXEC_DIRECTORY}

INCLUDEPATH += $${ROOT_DIRECTORY}/src
LIBS        += -L$${LIB_DIRECTORY} -lQEasyShell

SOURCES += \
    detachedexecution.cpp \
    asynchronous.cpp

HEADERS += \
    detachedexecution.h
