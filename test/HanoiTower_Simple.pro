QT += core widgets

# Qt 5.12.12 最简化配置
CONFIG += c++11
TARGET = HanoiTowerSimple
TEMPLATE = app

# 只包含最基本的源文件
SOURCES += \
    src/main_simple.cpp \
    src/mainwindow_simple.cpp

HEADERS += \
    src/mainwindow_simple.h

# 包含路径
INCLUDEPATH += src

# 输出目录
DESTDIR = bin
OBJECTS_DIR = build/obj
MOC_DIR = build/moc

# 确保目录存在
win32 {
    CONFIG += console
}

# 编译优化
QMAKE_CXXFLAGS += -Wall 