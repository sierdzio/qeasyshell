QT       = core
CONFIG   += console
CONFIG   -= app_bundle
TEMPLATE = app
DESTDIR  = $${EXEC_DIRECTORY}

INCLUDEPATH += $${ROOT_DIRECTORY}/src
LIBS        += -L$${EXEC_DIRECTORY} -lQEasyShell
