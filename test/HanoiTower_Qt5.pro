QT += core widgets

# Qt 5.12.12 专用配置
CONFIG += c++11
TARGET = HanoiTower
TEMPLATE = app

# 如果遇到编译错误，可以临时注释掉部分源文件
SOURCES += \
    src/main.cpp \
    src/mainwindow_simple.cpp \
    src/HanoiGame.cpp \
    src/gamecontroller.cpp

HEADERS += \
    src/mainwindow.h \
    include/HanoiGame.h \
    include/GameController.h

# 临时注释掉UI和资源文件，避免编译错误
# FORMS += forms/mainwindow.ui
# RESOURCES += resources/resources.qrc

# 包含路径
INCLUDEPATH += src include

# 输出目录
DESTDIR = bin
OBJECTS_DIR = build/obj
MOC_DIR = build/moc
UI_DIR = build/ui
RCC_DIR = build/rcc

# Windows特定配置
win32 {
    CONFIG += console
    RC_ICONS = icon.ico
}

# 确保创建输出目录
win32 {
    !exists($$DESTDIR) {
        system(mkdir $$DESTDIR)
    }
    !exists(build) {
        system(mkdir build)
    }
}

unix {
    !exists($$DESTDIR) {
        system(mkdir -p $$DESTDIR)
    }
    !exists(build) {
        system(mkdir -p build)
    }
} 