QT += core gui widgets

CONFIG += c++11
CONFIG += debug

TARGET = HanoiTower_MultiFile
TEMPLATE = app

SOURCES += \
    src/main_final.cpp \
    src/hanoigame_impl.cpp \
    src/gamedisplay.cpp \
    src/gamecontroller.cpp \
    src/mainwindow_multi.cpp

HEADERS += \
    src/hanoigame.h \
    src/gamedisplay.h \
    src/gamecontroller.h \
    src/mainwindow_multi.h

FORMS += \
    forms/mainwindow.ui

RESOURCES += \
    resources/resources.qrc

# Include directories
INCLUDEPATH += src 
