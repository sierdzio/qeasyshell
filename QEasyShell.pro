#-------------------------------------------------
#
# Project created by QtCreator 2012-10-20T16:13:25
#
#-------------------------------------------------

include(global.pri)

TARGET = QEasyShell
TEMPLATE = lib

DEFINES += QEASYSHELL_LIBRARY

SOURCES += qeasyshell.cpp

HEADERS += qeasyshell.h\
        QEasyShell_global.h

OTHER_FILES += \
    global.pri

#unix:!symbian {
#    target.path = /usr/lib
#    INSTALLS += target
#}
