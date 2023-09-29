QT += core gui widgets
CONFIG += c++11 plugin

TEMPLATE = lib
TARGET = FileRename
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
DEFINES += FILERENAME_LIB
HEADERS += ./*.h
SOURCES += ./*.cpp
RESOURCES += ./*.qrc
FORMS += ./*.ui

INCLUDEPATH += $$PWD/../../include
