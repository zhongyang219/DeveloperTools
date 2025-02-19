QT += core gui widgets webenginewidgets
CONFIG += c++11 plugin

TEMPLATE = lib
TARGET = KeyboardTester
CONFIG(debug, debug|release): {
    DESTDIR = $$PWD/../../bin/debug
}
else {
    DESTDIR = $$PWD/../../bin/release
}
DEFINES += KEYBOARDTESTER_LIB

INCLUDEPATH += $$PWD/../../include

HEADERS += \
    KeyboardTester.h \
    KeyboardTester_global.h

SOURCES += \
    KeyboardTester.cpp

RESOURCES += \
    keyboardTester.qrc
