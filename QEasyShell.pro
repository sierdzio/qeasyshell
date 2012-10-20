#-------------------------------------------------
#
# Project created by QtCreator 2012-10-20T16:13:25
#
# TODO: don't forget about those few lines:
#unix:!symbian {
#    target.path = /usr/lib
#    INSTALLS += target
#}
#-------------------------------------------------

include(global.pri)

TARGET = QEasyShell
TEMPLATE = lib

DEFINES += QEASYSHELL_LIBRARY

SOURCES += \
           qescommand.cpp \
           qesresult.cpp

HEADERS += \
           qeasyshellglobal.h \
           qescommand.h \
           qesdefinitions.h \
           qesresult.h

OTHER_FILES += \
               global.pri \
               QEasyShell
