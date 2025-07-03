QT += core gui widgets

CONFIG += c++11
CONFIG += debug

TARGET = HanoiTower
TEMPLATE = app

SOURCES += \
    src/main.cpp \
    src/hanoigame.cpp \
    src/gamedisplay.cpp \
    src/gamecontroller.cpp \
    src/mainwindow.cpp

HEADERS += \
    src/hanoigame.h \
    src/gamedisplay.h \
    src/gamecontroller.h \
    src/mainwindow.h

FORMS += \
    forms/mainwindow.ui

RESOURCES += \
    resources/resources.qrc

# Include directories
INCLUDEPATH += src 
