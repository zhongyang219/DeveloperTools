# ----------------------------------------------------
# This file is generated by the Qt Visual Studio Tools.
# ------------------------------------------------------

QT += core gui widgets
CONFIG += c++11 plugin

TEMPLATE = lib
TARGET = AddCodeHeader
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
#HEADERS += $$PWD/*.h
#SOURCES += $$PWD/*.cpp

INCLUDEPATH += $$PWD/../../include

HEADERS += \
    AddCodeHeader.h \
    AddCodeHeaderEditor.h \
    AddFileHeadThread.h \
    CRemoveCommentThread.h \
    CommonData.h \
    FileHeadHelper.h \
    HeadItemTableModel.h \
    MainWidget.h \
    RemoveCommentHelper.h \
    Test.h \
    addcodeheader_global.h

SOURCES += \
    AddCodeHeader.cpp \
    AddCodeHeaderEditor.cpp \
    AddFileHeadThread.cpp \
    CRemoveCommentThread.cpp \
    CommonData.cpp \
    FileHeadHelper.cpp \
    HeadItemTableModel.cpp \
    MainWidget.cpp \
    RemoveCommentHelper.cpp \
    Test.cpp
