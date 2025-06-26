# CodeBlocks + Qt 汉诺塔游戏配置指南

## 环境配置

### 1. Qt下载安装
按照上述步骤安装Qt，确保安装了MinGW编译器。

### 2. CodeBlocks配置

#### 设置编译器
```
Settings → Compiler → Create copy of GNU GCC Compiler
名称: Qt MinGW

Toolchain executables:
- Compiler's installation directory: C:\Qt\Tools\mingw1120_64\bin
- C++ compiler: g++.exe
- Linker: g++.exe
```

#### 设置全局变量
```
Settings → Global Variables
Variable: qt
Base: C:\Qt\6.5.0\mingw_64
Include: C:\Qt\6.5.0\mingw_64\include  
Lib: C:\Qt\6.5.0\mingw_64\lib
```

### 3. 项目配置

#### Build Options设置
```
Search directories → Compiler:
- $(#qt.include)
- $(#qt.include)/QtCore
- $(#qt.include)/QtWidgets
- $(#qt.include)/QtGui

Search directories → Linker:
- $(#qt.lib)

Linker settings → Link libraries:
- Qt6Core
- Qt6Widgets  
- Qt6Gui
```

#### 编译器标志
```
Compiler flags:
- 添加: -std=c++17

Other compiler options:
- -DQT_WIDGETS_LIB
- -DQT_GUI_LIB
- -DQT_CORE_LIB
```

## MOC处理方案

由于CodeBlocks不自动处理MOC，有两种解决方案：

### 方案一：使用外部构建脚本
创建 build.bat：
```batch
@echo off
echo Processing MOC files...

REM 设置Qt环境
set PATH=C:\Qt\6.5.0\mingw_64\bin;C:\Qt\Tools\mingw1120_64\bin;%PATH%

REM 处理MOC文件
moc src\mainwindow.h -o moc_mainwindow.cpp
moc src\hanoitower.h -o moc_hanoitower.cpp  
moc src\gamecontroller.h -o moc_gamecontroller.cpp

echo MOC files generated successfully!
echo Now you can build in CodeBlocks.
pause
```

### 方案二：简化版本（无信号槽）
如果想避免MOC复杂性，可以创建不使用Qt信号槽机制的简化版本。

## 推荐的开发流程

1. 运行 build.bat 生成MOC文件
2. 在CodeBlocks中编译项目
3. 测试运行
4. 如果修改了包含Q_OBJECT的头文件，重新运行build.bat

## 文件包含顺序
在源文件中按此顺序包含：
```cpp
#include "header.h"
#include "moc_header.cpp"  // 如果该头文件包含Q_OBJECT
``` 