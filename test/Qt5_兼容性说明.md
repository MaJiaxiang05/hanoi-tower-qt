# Qt 5.12.12 兼容性说明

## 常见编译错误及解决方案

### 1. C++标准版本问题
**错误**: `CONFIG += c++17` 不被支持
**解决**: 
- 已修改为 `CONFIG += c++11`
- Qt 5.12.12 默认支持C++11标准

### 2. MOC (Meta-Object Compiler) 问题
**可能错误**: MOC版本不匹配或无法生成moc文件
**解决方案**:
```bash
# 清理构建目录
rm -rf build/
# 重新运行qmake
qmake HanoiTower.pro
# 重新编译
make clean && make
```

### 3. 缺失的头文件包含
如果遇到"未定义的引用"错误，请确保以下头文件：

**mainwindow.cpp** 中需要包含：
```cpp
#include "mainwindow.h"
#include "ui_mainwindow.h"  // 如果使用UI文件
#include <QApplication>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
```

### 4. UI文件问题
**错误**: 找不到UI文件或UI类
**解决**: 确保 `forms/mainwindow.ui` 文件存在，或者删除UI相关代码：

在 `mainwindow.h` 中注释掉：
```cpp
// #include "ui_mainwindow.h"
```

在 `mainwindow.cpp` 中注释掉：
```cpp
// ui->setupUi(this);
```

### 5. 资源文件问题
**错误**: 找不到资源文件
**临时解决**: 注释掉资源相关行：
```pro
# RESOURCES += resources/resources.qrc
```

### 6. 信号槽连接语法
Qt 5.12.12 推荐使用新语法：
```cpp
// 推荐（新语法）
connect(button, &QPushButton::clicked, this, &MainWindow::onButtonClicked);

// 避免（旧语法，但仍可用）
connect(button, SIGNAL(clicked()), this, SLOT(onButtonClicked()));
```

## 快速修复步骤

### 步骤1: 创建最小可编译版本
如果遇到编译错误，先创建一个最小版本：

```cpp
// 简化的main.cpp
#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    QWidget window;
    QVBoxLayout* layout = new QVBoxLayout(&window);
    
    QLabel* label = new QLabel("汉诺塔游戏测试");
    QPushButton* button = new QPushButton("测试按钮");
    
    layout->addWidget(label);
    layout->addWidget(button);
    
    window.setWindowTitle("Qt 5.12.12 测试");
    window.show();
    
    return app.exec();
}
```

### 步骤2: 逐步添加功能
1. 首先确保基本窗口能正常显示
2. 然后添加游戏逻辑类
3. 最后添加图形界面

### 步骤3: 使用简化项目文件
使用 `HanoiTower_Qt5.pro` 替代原来的项目文件：

```bash
# 在Qt Creator中打开HanoiTower_Qt5.pro
# 或者在命令行中
qmake HanoiTower_Qt5.pro
make
```

## 编译步骤（Windows PowerShell）

```powershell
# 1. 清理之前的构建文件
Remove-Item -Recurse -Force build -ErrorAction SilentlyContinue
Remove-Item -Recurse -Force bin -ErrorAction SilentlyContinue

# 2. 重新生成Makefile
qmake HanoiTower_Qt5.pro

# 3. 编译项目
make

# 如果make命令不可用，使用nmake或mingw32-make
# nmake          # Visual Studio编译器
# mingw32-make   # MinGW编译器
```

## 编译步骤（Linux/macOS）

```bash
# 1. 清理之前的构建文件
rm -rf build bin

# 2. 重新生成Makefile
qmake HanoiTower_Qt5.pro

# 3. 编译项目
make
```

## 常见错误解决方案

### 错误1: "No rule to make target"
```bash
# 解决方案：清理并重新生成
make clean
qmake
make
```

### 错误2: "moc_*.cpp: No such file"
```bash
# 解决方案：手动生成MOC文件
moc src/mainwindow.h -o build/moc/moc_mainwindow.cpp
```

### 错误3: "QTime was not declared"
在相关文件中添加：
```cpp
#include <QTime>
```

### 错误4: "undefined reference to vtable"
这通常是MOC文件问题，解决方案：
```bash
# 删除所有构建文件
rm -rf build
# 重新运行qmake
qmake
make
```

## 调试建议

1. **从简单开始**: 先编译simple_hanoi.cpp控制台版本
2. **逐步调试**: 一次只编译一个模块
3. **检查依赖**: 确保所有头文件路径正确
4. **版本兼容**: 如果仍有问题，考虑升级到Qt 5.15或Qt 6

## 备用方案

如果Qt图形版本无法编译，可以先使用控制台版本：

```bash
# 编译控制台版本
g++ -o hanoi_console simple_hanoi.cpp
./hanoi_console
```

这样至少可以确保核心算法正常工作。 