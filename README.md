# 汉诺塔（Tower of Hanoi）- 多文件版本

一个用Qt5开发的汉诺塔游戏，支持可视化演示和手动操作。

##  项目特色

- **可视化界面**：使用Qt5创建的现代化图形界面
- **自动演示**：观看算法自动解决汉诺塔问题
- **手动操作**：用户可以手动移动圆盘
- **进度显示**：实时显示解题进度和步数
- **速度控制**：可调节演示速度

##  技术栈

- **语言**：C++
- **框架**：Qt 5.12.12
- **编译器**：MinGW 7.3.0
- **构建工具**：qmake

##  项目结构

```
Hanoi/
├── src/
│   ├── main_final.cpp         # 主程序入口
│   ├── gamecontroller.cpp     # 游戏控制器（核心逻辑）
│   ├── gamecontroller.h       # 游戏控制器（核心逻辑）
│   ├── gamedisplay.cpp        # 游戏显示组件
│   ├── gamedisplay.h          # 游戏显示组件
│   ├── hanoigame.h            # 游戏接口定义
│   ├── hanoigame_impl.cpp     # 汉诺塔游戏逻辑实现
│   ├── mainwindow.cpp         # 主窗口界面
│   └── mainwindow.h           # 主窗口界面
├── forms/
│   └── mainwindow.ui          # Qt Designer界面文件
├── resources/
│   └── resources.qrc          # 资源文件
├── HanoiTower_MultiFile.pro   # Qt项目文件
├── README.md                  # 项目说明
└── .gitignore                 # Git忽略文件
```

##  编译运行

### 环境要求
- Qt 5.12 或更高版本
- MinGW编译器
- Windows 10+

### 编译步骤
1. 克隆项目到本地：
   ```bash
   git clone https://github.com/MaJiaxiang05/hanoi-tower-qt.git
   cd hanoi-tower-qt
   ```

2. 使用Qt Creator：
   - 打开`HanoiTower_MultiFile.pro`
   - 配置编译环境（MinGW）
   - 点击构建按钮编译项目
   - 运行程序

3. 命令行编译：
   ```bash
   qmake HanoiTower_MultiFile.pro
   make
   ```

##  功能说明

### 主要功能
- **重置游戏**：重新开始游戏
- **自动演示**：观看完整的解题过程
- **暂停/继续**：控制演示进度
- **速度调节**：调整演示速度
- **手动操作**：点击圆盘进行移动
- **步数统计**：显示当前步数和最优解步数

### 核心算法
实现了经典的汉诺塔递归算法，可以自动生成最优解序列。

##  核心组件

### GameController
- 游戏流程控制
- 演示模式管理
- 用户输入处理
- 进度追踪

### HanoiGame
- 游戏逻辑核心
- 移动验证
- 状态管理
- 算法求解

### GameDisplay
- 可视化显示
- 用户交互界面
- 状态反馈

### MainWindow
- 主界面布局
- 控件管理
- 信号连接

##  技术亮点

- **QPair存储移动序列**：`QList<QPair<int, int>>`存储解决方案
- **QTimer控制演示节奏**：可调速度的自动演示
- **信号槽机制**：组件间松耦合通信
- **防御性编程**：空指针检查和错误处理
- **进度条同步**：确保演示完成时显示100%

##  开发说明

本项目采用模块化设计，将游戏逻辑、显示和控制分离：
- **数据层**：HanoiGame处理游戏状态和逻辑
- **显示层**：GameDisplay负责可视化
- **控制层**：GameController协调各组件
- **界面层**：MainWindow提供用户界面

##  许可证

本项目仅供学习交流使用。 
