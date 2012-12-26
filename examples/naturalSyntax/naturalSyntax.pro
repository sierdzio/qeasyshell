#-------------------------------------------------
#
# Project created by QtCreator 2012-12-26T16:42:00
#
#-------------------------------------------------

include(../../global.pri)

QT       = core

TARGET   = naturalSyntax
CONFIG   += console
CONFIG   -= app_bundle
TEMPLATE = app
DESTDIR  = $${EXEC_DIRECTORY}

INCLUDEPATH += $${ROOT_DIRECTORY}/src
LIBS        += -L$${LIB_DIRECTORY} -lQEasyShell

SOURCES += \
    naturalSyntax.cpp
