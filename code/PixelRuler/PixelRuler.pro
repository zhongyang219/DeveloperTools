# ----------------------------------------------------
# This file is generated by the Qt Visual Studio Tools.
# ------------------------------------------------------

QT += core gui widgets

TEMPLATE = lib
TARGET = PixelRuler
CONFIG(debug, debug|release): {
    DESTDIR = $$PWD/../../bin/debug
    LIBS += -L$$PWD/../../bin/debug \
        -lCCommonTools
}
else {
    DESTDIR = $$PWD/../../bin/release
    LIBS += -L$$PWD/../../bin/release \
        -lCCommonTools
}
DEFINES += ADDCODEHEADER_LIB
HEADERS += ./*.h
SOURCES += ./*.cpp

INCLUDEPATH += $$PWD/../../include
