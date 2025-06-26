QT += core gui widgets

# Qt Creator 5.0.2 专用配置
CONFIG += c++98  # 使用更旧的C++标准
TARGET = HanoiTowerQt502
TEMPLATE = app

# 只包含界面相关的源文件
SOURCES += \
    src/main_simple.cpp \
    src/mainwindow_simple.cpp

HEADERS += \
    src/mainwindow_simple.h

# 包含路径
INCLUDEPATH += src

# 输出目录（使用相对路径）
DESTDIR = bin
OBJECTS_DIR = obj
MOC_DIR = moc

# Qt 5.0.2 兼容设置
DEFINES += QT_NO_DEBUG_OUTPUT

# 编译器设置
win32 {
    # Windows特定设置
    CONFIG += console
}

unix {
    # Linux/Mac设置
    CONFIG += debug_and_release
}

# 移除可能导致问题的优化标志
QMAKE_CXXFLAGS -= -Wall 