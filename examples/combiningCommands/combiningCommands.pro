#-------------------------------------------------
#
# Project created by QtCreator 2012-11-11T12:35:31
#
#-------------------------------------------------

include(../../global.pri)

QT       = core

TARGET   = combiningCommands
CONFIG   += console
CONFIG   -= app_bundle
TEMPLATE = app
DESTDIR  = $${EXEC_DIRECTORY}

INCLUDEPATH += $${ROOT_DIRECTORY}/src
LIBS        += -L$${LIB_DIRECTORY} -lQEasyShell

SOURCES += main.cpp
