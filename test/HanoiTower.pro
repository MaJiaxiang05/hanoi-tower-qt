QT += core widgets

# Qt 5.12.12 兼容配置
CONFIG += c++11
TARGET = HanoiTower
TEMPLATE = app

# 源文件
SOURCES += \
    src/main.cpp \
    src/mainwindow.cpp \
    src/hanoitower.cpp \
    src/disk.cpp \
    src/tower.cpp \
    src/gamecontroller.cpp

# 头文件
HEADERS += \
    src/mainwindow.h \
    src/hanoitower.h \
    src/disk.h \
    src/tower.h \
    src/gamecontroller.h

# UI文件
FORMS += \
    forms/mainwindow.ui

# 资源文件
RESOURCES += \
    resources/resources.qrc

# 设置输出目录
DESTDIR = bin
OBJECTS_DIR = build/obj
MOC_DIR = build/moc
UI_DIR = build/ui
RCC_DIR = build/rcc

# 编译器配置
win32 {
    CONFIG += console
}

# 包含路径
INCLUDEPATH += src 