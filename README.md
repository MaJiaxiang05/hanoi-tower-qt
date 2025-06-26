# 汉诺塔（Tower of Hanoi）

一个用Qt5开发的汉诺塔游戏，支持可视化演示和手动操作。

## 🎮 项目特色

- **可视化界面**：使用Qt5创建的现代化图形界面
- **自动演示**：观看算法自动解决汉诺塔问题
- **手动操作**：用户可以手动移动圆盘
- **进度显示**：实时显示解题进度
- **多种难度**：支持不同层数的汉诺塔

## 🛠️ 技术栈

- **语言**：C++
- **框架**：Qt 5.12.12
- **编译器**：MinGW 7.3.0
- **构建工具**：qmake

## 📁 项目结构

```
Hanoi/
├── src/                    # 源代码文件
│   ├── main_final.cpp     # 主程序入口
│   ├── gamecontroller.*   # 游戏控制器
│   ├── gamedisplay.*      # 游戏显示组件
│   ├── hanoigame_impl.*   # 汉诺塔游戏逻辑
│   └── mainwindow_multi.* # 主窗口
├── forms/                 # UI界面文件
├── resources/             # 资源文件
└── HanoiTower_MultiFile.pro # Qt项目文件
```

## 🚀 编译运行

### 环境要求
- Qt 5.12 或更高版本
- MinGW编译器
- Windows 10+

### 编译步骤
1. 克隆项目到本地
2. 使用Qt Creator打开`HanoiTower_MultiFile.pro`
3. 配置编译环境（MinGW）
4. 点击构建按钮编译项目
5. 运行程序

### 命令行编译
```bash
qmake HanoiTower_MultiFile.pro
make
```

## 🎯 功能说明

### 主要功能
- **重置游戏**：重新开始游戏
- **自动演示**：观看完整的解题过程
- **暂停/继续**：控制演示速度
- **手动操作**：点击圆盘进行移动

### 核心算法
实现了经典的汉诺塔递归算法：
```cpp
void solveHanoi(int n, int source, int target, int auxiliary)
```

## 🧩 核心组件

### GameController
- 游戏流程控制
- 演示模式管理
- 用户输入处理

### HanoiGame
- 游戏逻辑核心
- 移动验证
- 状态管理

### GameDisplay
- 可视化显示
- 动画效果
- 用户界面

## 📝 开发笔记

项目经历了从HanoiTower系统到HanoiGame系统的重构，采用了更清晰的架构设计。

### 技术亮点
- 使用QPair存储移动序列
- QTimer控制演示节奏
- 信号槽机制实现组件通信
- 防御性编程避免空指针异常

## 👨‍💻 作者

开发过程中得到了AI编程助手的协助，解决了多个技术难题。

## 📄 许可证

本项目仅供学习交流使用。 