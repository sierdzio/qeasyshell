#-------------------------------------------------
#
# Project created by QtCreator 2012-12-26T15:31:38
#
#-------------------------------------------------

include(../../global.pri)

QT       = core

TARGET   = inputData
CONFIG   += console
CONFIG   -= app_bundle
TEMPLATE = app
DESTDIR  = $${EXEC_DIRECTORY}

INCLUDEPATH += $${ROOT_DIRECTORY}/src
LIBS        += -L$${LIB_DIRECTORY} -lQEasyShell

SOURCES += \
    inputData.cpp
