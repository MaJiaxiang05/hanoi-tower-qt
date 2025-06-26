# 汉诺塔游戏编译指南

## 环境准备

### 1. 安装Qt开发环境

#### Windows
1. 下载Qt官方安装器：[https://www.qt.io/download](https://www.qt.io/download)
2. 运行安装器，选择以下组件：
   - Qt 6.5.x (或最新稳定版本)
   - MinGW 11.2.0 64-bit (或MSVC 2019/2022)
   - Qt Creator
   - CMake
   - Ninja

#### macOS
```bash
# 使用Homebrew安装
brew install qt6
brew install qt-creator
```

#### Linux (Ubuntu/Debian)
```bash
# 安装Qt6开发包
sudo apt update
sudo apt install qt6-base-dev qt6-tools-dev qt6-tools-dev-tools
sudo apt install qtcreator cmake build-essential
```

### 2. 验证安装
```bash
# 检查Qt版本
qmake --version

# 检查编译器
g++ --version  # 或 clang++ --version
```

## 编译方法

### 方法一：使用Qt Creator (推荐)

1. **打开Qt Creator**
2. **打开项目**
   - 文件 → 打开文件或项目
   - 选择 `HanoiTower.pro` 文件
3. **配置构建套件**
   - Qt Creator会自动检测可用的构建套件
   - 选择合适的Qt版本和编译器
4. **编译项目**
   - 点击左下角的"构建"按钮 (🔨图标)
   - 或按快捷键 `Ctrl+B` (Windows/Linux) 或 `Cmd+B` (macOS)
5. **运行程序**
   - 点击"运行"按钮 (▶️图标)
   - 或按快捷键 `Ctrl+R` (Windows/Linux) 或 `Cmd+R` (macOS)

### 方法二：命令行编译

#### Windows (MinGW)
```powershell
# 设置Qt环境变量（如果需要）
set PATH=C:\Qt\6.5.0\mingw_64\bin;%PATH%

# 进入项目目录
cd HanoiTower

# 生成Makefile
qmake HanoiTower.pro

# 编译
mingw32-make

# 运行
.\bin\HanoiTower.exe
```

#### macOS/Linux
```bash
# 进入项目目录
cd HanoiTower

# 生成Makefile
qmake HanoiTower.pro

# 编译
make

# 运行
./bin/HanoiTower
```

### 方法三：使用CMake

如果你更喜欢CMake，可以创建 `CMakeLists.txt`：

```cmake
cmake_minimum_required(VERSION 3.16)
project(HanoiTower)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

qt6_standard_project_setup()

set(SOURCES
    src/main.cpp
    src/mainwindow.cpp
    src/hanoitower.cpp
    src/disk.cpp
    src/tower.cpp
    src/gamecontroller.cpp
)

set(HEADERS
    src/mainwindow.h
    src/hanoitower.h
    src/disk.h
    src/tower.h
    src/gamecontroller.h
)

qt6_add_executable(HanoiTower ${SOURCES} ${HEADERS})
qt6_add_resources(HanoiTower "resources" PREFIX "/" FILES resources/resources.qrc)

target_link_libraries(HanoiTower Qt6::Core Qt6::Widgets)

set_target_properties(HanoiTower PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)
```

然后使用CMake编译：
```bash
mkdir build
cd build
cmake ..
make
```

## 常见编译问题

### 1. Qt版本不匹配
**错误信息**：`Project ERROR: Unknown module(s) in QT: widgets`
**解决方案**：
- 确保安装了Qt Widgets模块
- 检查Qt版本是否正确

### 2. 编译器路径问题
**错误信息**：`qmake: command not found`
**解决方案**：
- 将Qt的bin目录添加到PATH环境变量
- Windows: `C:\Qt\6.5.0\mingw_64\bin`
- macOS: `/usr/local/opt/qt6/bin`
- Linux: `/usr/bin` (如果用包管理器安装)

### 3. C++17支持问题
**错误信息**：编译器不支持C++17特性
**解决方案**：
- 确保使用支持C++17的编译器版本
- GCC 7+, Clang 5+, MSVC 2017+

### 4. MOC文件生成失败
**错误信息**：`moc: No such file or directory`
**解决方案**：
- 清理项目：`make clean` 或在Qt Creator中选择"清理"
- 重新生成MOC文件：`qmake` 然后重新编译

### 5. 链接错误
**错误信息**：未定义的引用错误
**解决方案**：
- 检查.pro文件中的QT模块设置
- 确保所有源文件都在SOURCES中列出
- 重新运行qmake

## 编译输出

编译成功后，你会在以下位置找到可执行文件：
- **Windows**: `bin/HanoiTower.exe`
- **macOS**: `bin/HanoiTower.app`
- **Linux**: `bin/HanoiTower`

## 调试模式

要编译调试版本：

### Qt Creator
- 在左侧选择"Debug"配置
- 编译并运行

### 命令行
```bash
# 生成Debug版本的Makefile
qmake CONFIG+=debug HanoiTower.pro

# 编译
make
```

## 发布版本

要编译发布版本：

### Qt Creator
- 在左侧选择"Release"配置
- 编译

### 命令行
```bash
# 生成Release版本的Makefile
qmake CONFIG+=release HanoiTower.pro

# 编译
make
```

## 部署

### Windows
使用Qt提供的windeployqt工具：
```cmd
windeployqt.exe bin/HanoiTower.exe
```

### macOS
使用macdeployqt工具：
```bash
macdeployqt bin/HanoiTower.app
```

### Linux
手动复制所需的Qt库，或使用linuxdeployqt工具。

## 自动化构建

你可以创建一个简单的构建脚本：

### build.sh (Linux/macOS)
```bash
#!/bin/bash
echo "开始编译汉诺塔游戏..."

# 清理之前的构建
make clean 2>/dev/null || true
rm -rf bin build

# 生成Makefile
qmake HanoiTower.pro

# 编译项目
if make; then
    echo "编译成功！可执行文件位于: bin/HanoiTower"
else
    echo "编译失败！"
    exit 1
fi
```

### build.bat (Windows)
```batch
@echo off
echo 开始编译汉诺塔游戏...

REM 清理之前的构建
if exist Makefile (
    mingw32-make clean
)
if exist bin rmdir /s /q bin
if exist build rmdir /s /q build

REM 生成Makefile
qmake HanoiTower.pro

REM 编译项目
mingw32-make
if %errorlevel% == 0 (
    echo 编译成功！可执行文件位于: bin\HanoiTower.exe
) else (
    echo 编译失败！
    exit /b 1
)
```

运行构建脚本：
```bash
# Linux/macOS
chmod +x build.sh
./build.sh

# Windows
build.bat
```

---

现在你应该能够成功编译和运行汉诺塔游戏了！如果遇到问题，请检查Qt版本和编译器设置。 