# ----------------------------------------------------
# This file is generated by the Qt Visual Studio Tools.
# ------------------------------------------------------

QT += core gui widgets
CONFIG += c++11 plugin
TEMPLATE = lib
TARGET = WallpaperTool
DEFINES += WALLPAPERTOOL_LIB
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

win32{
    LIBS += -lOle32 -lAdvapi32
}

#HEADERS += $$PWD/*.h
#SOURCES += $$PWD/*.cpp
#FORMS += $$PWD/*.ui

INCLUDEPATH += $$PWD/../../include

FORMS +=

HEADERS += \
    HistoryWallpaperMgr.h \
    HistoryWallpaperWidget.h \
    ImageLabel.h \
    MainWidget.h \
    SettingsDlg.h \
    WallpapaerWidget.h \
    WallpaperHelper.h \
    WallpaperTool.h \
    wallpapertool_global.h

SOURCES += \
    HistoryWallpaperMgr.cpp \
    HistoryWallpaperWidget.cpp \
    ImageLabel.cpp \
    MainWidget.cpp \
    SettingsDlg.cpp \
    WallpapaerWidget.cpp \
    WallpaperHelper.cpp \
    WallpaperTool.cpp
