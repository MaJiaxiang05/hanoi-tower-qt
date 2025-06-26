#include "mainwindow_simple.h"
#include <QApplication>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QMessageBox>
#include <QPushButton>
#include <QLabel>
#include <QSpinBox>
#include <QSlider>
#include <QProgressBar>
#include <QTextEdit>
#include <QGroupBox>
#include <QSplitter>
#include <QMenuBar>
#include <QStatusBar>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QGraphicsEllipseItem>
#include <QGraphicsTextItem>
#include <QBrush>
#include <QPen>
#include <QFont>
#include <QTime>
#include <QKeySequence>
#include <QMenu>
#include <QAction>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_game(nullptr)
    , m_controller(nullptr)
    , m_centralWidget(nullptr)
    , m_gameView(nullptr)
    , m_gameScene(nullptr)
    , m_selectedTower(-1)
    , m_isDemonstrating(false)
    , m_diskCount(3)
    , m_moveCount(0)
    , m_currentStep(0)
    , m_demoTimer(NULL)
{
    // 初始化游戏数据
    initializeGame();
    // 先设置UI界面
    setupUI();
    
    // 初始化演示定时器
    m_demoTimer = new QTimer(this);
    connect(m_demoTimer, SIGNAL(timeout()), this, SLOT(executeDemoStep()));
    
    // Qt 5.0.2兼容：暂时不初始化游戏逻辑类
    // 因为HanoiTower和GameController类可能包含不兼容的代码
    m_game = nullptr;
    m_controller = nullptr;
    
    qDebug() << "Qt 5.0.2版本 - 暂时只显示界面，游戏逻辑待后续实现";
    
    // UI创建完成后连接信号槽
    setupConnections();
    
    // 更新显示
    updateGameDisplay();
    updateControlButtons();
}

MainWindow::~MainWindow()
{
    // Qt会自动清理子对象
}

void MainWindow::setupUI()
{
    // 创建中央窗口部件
    m_centralWidget = new QWidget;
    setCentralWidget(m_centralWidget);
    
    // 创建主分割器
    m_mainSplitter = new QSplitter(Qt::Horizontal);
    
    // 设置游戏显示区域
    setupGameArea();
    
    // 添加到分割器
    m_mainSplitter->addWidget(m_gameView);
    m_mainSplitter->addWidget(createControlWidget());
    
    // 设置分割器比例
    m_mainSplitter->setStretchFactor(0, 3);  // 游戏区域占更多空间
    m_mainSplitter->setStretchFactor(1, 1);  // 控制面板
    
    // 设置主布局
    QHBoxLayout* mainLayout = new QHBoxLayout;
    mainLayout->addWidget(m_mainSplitter);
    m_centralWidget->setLayout(mainLayout);
    
    // 设置菜单栏
    setupMenuBar();
    
    // 设置状态栏
    setupStatusBar();
    
    // 设置窗口属性
    setWindowTitle("汉诺塔游戏");
    resize(1000, 600);
    setMinimumSize(800, 500);
}

void MainWindow::setupGameArea()
{
    // 创建图形视图和场景
    m_gameScene = new QGraphicsScene(this);
    m_gameScene->setSceneRect(0, 0, SCENE_WIDTH, SCENE_HEIGHT);
    
    m_gameView = new QGraphicsView(m_gameScene);
    m_gameView->setRenderHint(QPainter::Antialiasing);
    m_gameView->setBackgroundBrush(QBrush(QColor(240, 248, 255))); // 浅蓝色背景
    m_gameView->setMinimumSize(600, 400);
    
    // 添加柱子和盘子的显示
    drawTowersAndDisks();
}

QWidget* MainWindow::createControlWidget()
{
    QWidget* controlWidget = new QWidget;
    QVBoxLayout* layout = new QVBoxLayout(controlWidget);
    
    // 演示控制组
    m_controlGroup = new QGroupBox("演示控制");
    QVBoxLayout* demoLayout = new QVBoxLayout(m_controlGroup);
    
    m_startDemoButton = new QPushButton("开始演示");
    m_pauseButton = new QPushButton("暂停");
    m_stopButton = new QPushButton("停止");
    m_resetButton = new QPushButton("重置游戏");
    
    m_pauseButton->setEnabled(false);
    m_stopButton->setEnabled(false);
    
    demoLayout->addWidget(m_startDemoButton);
    demoLayout->addWidget(m_pauseButton);
    demoLayout->addWidget(m_stopButton);
    demoLayout->addWidget(m_resetButton);
    
    // 游戏设置组
    m_settingsGroup = new QGroupBox("游戏设置");
    QVBoxLayout* settingsLayout = new QVBoxLayout(m_settingsGroup);
    
    // 盘子数量设置
    QHBoxLayout* diskLayout = new QHBoxLayout;
    diskLayout->addWidget(new QLabel("盘子数量:"));
    m_diskCountSpinBox = new QSpinBox;
    m_diskCountSpinBox->setRange(1, 10);
    m_diskCountSpinBox->setValue(3);
    diskLayout->addWidget(m_diskCountSpinBox);
    settingsLayout->addLayout(diskLayout);
    
    // 演示速度设置
    QHBoxLayout* speedLayout = new QHBoxLayout;
    speedLayout->addWidget(new QLabel("演示速度:"));
    m_speedSlider = new QSlider(Qt::Horizontal);
    m_speedSlider->setRange(1, 10);
    m_speedSlider->setValue(5);
    m_speedLabel = new QLabel("5");
    speedLayout->addWidget(m_speedSlider);
    speedLayout->addWidget(m_speedLabel);
    settingsLayout->addLayout(speedLayout);
    
    // 手动操作组
    m_manualGroup = new QGroupBox("手动操作");
    QHBoxLayout* manualLayout = new QHBoxLayout(m_manualGroup);
    
    m_towerAButton = new QPushButton("柱子 A");
    m_towerBButton = new QPushButton("柱子 B");
    m_towerCButton = new QPushButton("柱子 C");
    
    manualLayout->addWidget(m_towerAButton);
    manualLayout->addWidget(m_towerBButton);
    manualLayout->addWidget(m_towerCButton);
    
    // 信息显示组
    m_infoGroup = new QGroupBox("游戏信息");
    QVBoxLayout* infoLayout = new QVBoxLayout(m_infoGroup);
    
    m_moveCountLabel = new QLabel("移动步数: 0");
    m_optimalMovesLabel = new QLabel("最优步数: 7");
    m_progressBar = new QProgressBar;
    m_progressBar->setRange(0, 100);
    m_progressBar->setValue(0);
    
    infoLayout->addWidget(m_moveCountLabel);
    infoLayout->addWidget(m_optimalMovesLabel);
    infoLayout->addWidget(new QLabel("演示进度:"));
    infoLayout->addWidget(m_progressBar);
    
    // 日志显示组
    m_logGroup = new QGroupBox("操作日志");
    QVBoxLayout* logLayout = new QVBoxLayout(m_logGroup);
    
    m_logTextEdit = new QTextEdit;
    m_logTextEdit->setMaximumHeight(150);
    m_logTextEdit->setReadOnly(true);
    logLayout->addWidget(m_logTextEdit);
    
    // 添加所有组到主布局
    layout->addWidget(m_controlGroup);
    layout->addWidget(m_settingsGroup);
    layout->addWidget(m_manualGroup);
    layout->addWidget(m_infoGroup);
    layout->addWidget(m_logGroup);
    layout->addStretch(); // 添加弹性空间
    
    return controlWidget;
}

void MainWindow::setupMenuBar()
{
    // 创建菜单
    QMenu* gameMenu = menuBar()->addMenu("游戏(&G)");
    QMenu* helpMenu = menuBar()->addMenu("帮助(&H)");
    
    // 游戏菜单项
    QAction* resetAction = new QAction("重置游戏(&R)", this);
    resetAction->setShortcut(QKeySequence("Ctrl+R"));
    
    m_exitAction = new QAction("退出(&X)", this);
    m_exitAction->setShortcut(QKeySequence("Alt+F4"));
    
    gameMenu->addAction(resetAction);
    gameMenu->addSeparator();
    gameMenu->addAction(m_exitAction);
    
    // 帮助菜单项
    m_howToPlayAction = new QAction("游戏规则(&H)", this);
    m_aboutAction = new QAction("关于(&A)", this);
    
    helpMenu->addAction(m_howToPlayAction);
    helpMenu->addSeparator();
    helpMenu->addAction(m_aboutAction);
    
    // Qt 5.0.2兼容：使用旧式信号槽语法
    connect(resetAction, SIGNAL(triggered()), this, SLOT(onResetGame()));
    connect(m_exitAction, SIGNAL(triggered()), this, SLOT(close()));
    connect(m_howToPlayAction, SIGNAL(triggered()), this, SLOT(onHowToPlay()));
    connect(m_aboutAction, SIGNAL(triggered()), this, SLOT(onAbout()));
}

void MainWindow::setupStatusBar()
{
    statusBar()->showMessage("就绪");
}

void MainWindow::setupConnections()
{
    // Qt 5.0.2兼容：使用旧式信号槽语法
    // 连接控制按钮信号槽
    connect(m_startDemoButton, SIGNAL(clicked()), this, SLOT(onStartDemonstration()));
    connect(m_pauseButton, SIGNAL(clicked()), this, SLOT(onPauseDemonstration()));
    connect(m_stopButton, SIGNAL(clicked()), this, SLOT(onStopDemonstration()));
    connect(m_resetButton, SIGNAL(clicked()), this, SLOT(onResetGame()));
    
    // 连接设置控件信号槽
    connect(m_diskCountSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onDiskCountChanged(int)));
    connect(m_speedSlider, SIGNAL(valueChanged(int)), this, SLOT(onSpeedChanged(int)));
    
    // 连接手动操作按钮
    connect(m_towerAButton, SIGNAL(clicked()), this, SLOT(onTowerAClicked()));
    connect(m_towerBButton, SIGNAL(clicked()), this, SLOT(onTowerBClicked()));
    connect(m_towerCButton, SIGNAL(clicked()), this, SLOT(onTowerCClicked()));
}

void MainWindow::updateGameDisplay()
{
    // 使用真实的游戏数据
    m_moveCountLabel->setText(QString("移动步数: %1").arg(m_moveCount));
    
    // 计算最优步数 (2^n - 1) - Qt 5.0.2兼容写法
    int optimal = 1;
    int n;
    for (n = 0; n < m_diskCount; ++n) {
        optimal *= 2;
    }
    optimal -= 1;
    m_optimalMovesLabel->setText(QString("最优步数: %1").arg(optimal));
    
    // 重新绘制游戏界面
    drawTowersAndDisks();
}

void MainWindow::updateControlButtons()
{
    bool demonstrating = m_isDemonstrating;
    
    m_startDemoButton->setEnabled(!demonstrating);
    m_pauseButton->setEnabled(demonstrating);
    m_stopButton->setEnabled(demonstrating);
    
    m_diskCountSpinBox->setEnabled(!demonstrating);
    
    m_towerAButton->setEnabled(!demonstrating);
    m_towerBButton->setEnabled(!demonstrating);
    m_towerCButton->setEnabled(!demonstrating);
}

void MainWindow::logMessage(const QString &message)
{
    if (m_logTextEdit) {
        m_logTextEdit->append(QString("[%1] %2")
                             .arg(QTime::currentTime().toString("hh:mm:ss"))
                             .arg(message));
    }
}

// 槽函数实现
void MainWindow::onStartDemonstration()
{
    logMessage("开始演示最优解");
    
    // 重置游戏到初始状态
    resetGameData();
    
    // 生成解决方案
    generateSolution();
    
    if (m_solution.isEmpty()) {
        logMessage("无法生成解决方案");
        return;
    }
    
    m_isDemonstrating = true;
    m_currentStep = 0;
    updateControlButtons();
    
    // 开始演示（根据速度调整间隔）
    int speed = m_speedSlider->value();
    int interval = 2000 - (speed - 1) * 200;  // 速度1=1800ms, 速度10=200ms
    m_demoTimer->start(interval);
    
    logMessage(QString("开始演示，共%1步").arg(m_solution.size()));
}

void MainWindow::onStopDemonstration()
{
    logMessage("停止演示");
    m_demoTimer->stop();
    m_isDemonstrating = false;
    m_currentStep = 0;
    updateControlButtons();
    
    // 清空进度条
    if (m_progressBar) {
        m_progressBar->setValue(0);
    }
}

void MainWindow::onPauseDemonstration()
{
    if (m_demoTimer->isActive()) {
        m_demoTimer->stop();
        m_pauseButton->setText("继续");
        logMessage("暂停演示");
    } else {
        int speed = m_speedSlider->value();
        int interval = 2000 - (speed - 1) * 200;
        m_demoTimer->start(interval);
        m_pauseButton->setText("暂停");
        logMessage("继续演示");
    }
}

void MainWindow::onResetGame()
{
    logMessage("重置游戏");
    resetGameData();
    updateControlButtons();
    updateGameDisplay();
}

void MainWindow::onDiskCountChanged(int count)
{
    logMessage(QString("设置盘子数量为: %1").arg(count));
    m_diskCount = count;
    resetGameData();
    updateGameDisplay();
}

void MainWindow::onSpeedChanged(int speed)
{
    m_speedLabel->setText(QString::number(speed));
}

void MainWindow::onTowerAClicked()
{
    selectTower(0);
}

void MainWindow::onTowerBClicked()
{
    selectTower(1);
}

void MainWindow::onTowerCClicked()
{
    selectTower(2);
}

void MainWindow::selectTower(int towerIndex)
{
    if (m_selectedTower == -1) {
        // 第一次选择
        m_selectedTower = towerIndex;
        logMessage(QString("选择了柱子 %1").arg(char('A' + towerIndex)));
        updateGameDisplay(); // 显示选中状态
    } else {
        // 第二次选择，尝试移动
        if (m_selectedTower != towerIndex) {
            if (moveDiskBetweenTowers(m_selectedTower, towerIndex)) {
                // 检查游戏是否胜利
                if (checkGameWon()) {
                    onGameWon();
                }
            }
        }
        m_selectedTower = -1;
        updateGameDisplay();
    }
}

bool MainWindow::checkGameWon()
{
    // 检查是否所有盘子都在第三个柱子上
    return m_towers[2].size() == m_diskCount;
}

void MainWindow::onGameWon()
{
    showGameWonDialog();
}

void MainWindow::showGameWonDialog()
{
    // Qt 5.0.2兼容：计算最优步数
    int optimal = 1;
    int n;
    for (n = 0; n < m_diskCount; ++n) {
        optimal *= 2;
    }
    optimal -= 1;
    
    // 计算效率，避免除零
    int efficiency = (m_moveCount > 0) ? (optimal * 100 / m_moveCount) : 100;
    
    QMessageBox::information(this, "恭喜!", 
                           QString("游戏完成!\n用了 %1 步\n最优步数: %2\n效率: %3%")
                           .arg(m_moveCount)
                           .arg(optimal)
                           .arg(efficiency));
}

void MainWindow::onMoveCountChanged(int count)
{
    updateGameDisplay();
}

void MainWindow::onDemonstrationStepExecuted(int step, int from, int to)
{
    logMessage(QString("第%1步: 从柱子%2移到柱子%3")
               .arg(step + 1)
               .arg(char('A' + from))
               .arg(char('A' + to)));
}

void MainWindow::onDemonstrationFinished()
{
    logMessage("演示完成!");
    m_isDemonstrating = false;
    updateControlButtons();
}

void MainWindow::onInvalidMoveAttempted(const QString &message)
{
    logMessage(QString("无效移动: %1").arg(message));
}

void MainWindow::onAbout()
{
    QMessageBox::about(this, "关于汉诺塔游戏",
                      "汉诺塔游戏 v1.0\n\n"
                      "这是一个经典的递归算法演示程序。\n"
                      "目标是将所有盘子从左边移动到右边，\n"
                      "遵循以下规则：\n"
                      "1. 一次只能移动一个盘子\n"
                      "2. 只能移动柱子顶部的盘子\n"
                      "3. 大盘子不能放在小盘子上面");
}

void MainWindow::onHowToPlay()
{
    QMessageBox::information(this, "游戏规则",
                            "游戏规则：\n\n"
                            "1. 点开始演示观看自动演示\n"
                            "2. 手动操作：先点击起始柱子，再点击目标柱子\n"
                            "3. 目标：将所有盘子移动到最右边的柱子\n"
                            "4. 限制：大盘子不能放在小盘子上面\n\n"
                            "快捷键：\n"
                            "Ctrl+R - 重置游戏\n"
                            "Alt+F4 - 退出游戏");
}

// 游戏逻辑实现
void MainWindow::initializeGame()
{
    m_towers.clear();
    int i;
    for (i = 0; i < 3; ++i) {
        m_towers.append(QList<int>());
    }
    
    // 将所有盘子放在第一个柱子上，从大到小
    for (i = m_diskCount; i >= 1; --i) {
        m_towers[0].append(i);
    }
    
    m_moveCount = 0;
}

void MainWindow::resetGameData()
{
    // 停止演示
    if (m_demoTimer) {
        m_demoTimer->stop();
    }
    
    initializeGame();
    m_selectedTower = -1;
    m_isDemonstrating = false;
    m_currentStep = 0;
    m_solution.clear();
    
    // 重置暂停按钮文本
    if (m_pauseButton) {
        m_pauseButton->setText("暂停");
    }
    
    // 清空进度条
    if (m_progressBar) {
        m_progressBar->setValue(0);
    }
}

bool MainWindow::moveDiskBetweenTowers(int from, int to)
{
    if (from < 0 || from >= 3 || to < 0 || to >= 3 || from == to) {
        return false;
    }
    
    if (m_towers[from].isEmpty()) {
        logMessage(QString("柱子 %1 上没有盘子").arg(char('A' + from)));
        return false;
    }
    
    int disk = m_towers[from].last();
    
    if (!m_towers[to].isEmpty() && m_towers[to].last() < disk) {
        logMessage(QString("不能将大盘子放在小盘子上"));
        return false;
    }
    
    // 执行移动
    m_towers[from].removeLast();
    m_towers[to].append(disk);
    m_moveCount++;
    
    // 更新图形显示
    updateGameDisplay();
    
    logMessage(QString("移动盘子 %1 从柱子 %2 到柱子 %3")
               .arg(disk)
               .arg(char('A' + from))
               .arg(char('A' + to)));
    
    return true;
}

// 图形绘制实现
void MainWindow::drawTowersAndDisks()
{
    // 清除现有内容
    m_gameScene->clear();
    
    // 绘制底座
    QGraphicsRectItem* base = m_gameScene->addRect(50, 350, 700, 20, 
                                                  QPen(Qt::black, 2), 
                                                  QBrush(Qt::darkGray));
    
    // 绘制三个柱子
    int i;
    for (i = 0; i < 3; ++i) {
        drawTower(i, 150 + i * 250, 150);
    }
    
    // 绘制盘子
    int towerIndex, diskIndex;
    for (towerIndex = 0; towerIndex < 3; ++towerIndex) {
        for (diskIndex = 0; diskIndex < m_towers[towerIndex].size(); ++diskIndex) {
            int diskSize = m_towers[towerIndex][diskIndex];
            drawDisk(diskSize, towerIndex, diskIndex);
        }
    }
    
    // 添加柱子标签
    for (i = 0; i < 3; ++i) {
        QGraphicsTextItem* label = m_gameScene->addText(QString("柱子 %1").arg(char('A' + i)));
        label->setPos(120 + i * 250, 380);
        label->setFont(QFont("Arial", 12, QFont::Bold));
    }
    
    // 显示选中状态
    if (m_selectedTower >= 0 && m_selectedTower < 3) {
        QGraphicsRectItem* highlight = m_gameScene->addRect(100 + m_selectedTower * 250, 140, 
                                                           100, 240, 
                                                           QPen(Qt::red, 3, Qt::DashLine), 
                                                           QBrush(Qt::transparent));
    }
}

void MainWindow::drawTower(int towerIndex, int x, int y)
{
    // 绘制柱子（垂直的矩形）- Qt 5.0.2兼容颜色
    QColor brownColor(139, 69, 19);  // 棕色RGB值
    QGraphicsRectItem* tower = m_gameScene->addRect(x - 5, y, 10, 200, 
                                                   QPen(Qt::black, 2), 
                                                   QBrush(brownColor));
}

void MainWindow::drawDisk(int size, int towerIndex, int position)
{
    int baseWidth = 30;
    int width = baseWidth + size * 20;  // 盘子宽度根据大小变化
    int height = 20;                    // 盘子高度
    
    int x = 150 + towerIndex * 250 - width / 2;  // 居中对齐
    int y = 330 - position * height;              // 从底部向上堆叠
    
    // Qt 5.0.2兼容：使用switch语句替代数组初始化
    QColor diskColor;
    switch ((size - 1) % 10) {
        case 0: diskColor = Qt::red; break;
        case 1: diskColor = Qt::blue; break;
        case 2: diskColor = Qt::green; break;
        case 3: diskColor = Qt::yellow; break;
        case 4: diskColor = Qt::magenta; break;
        case 5: diskColor = Qt::cyan; break;
        case 6: diskColor = Qt::darkRed; break;
        case 7: diskColor = Qt::darkBlue; break;
        case 8: diskColor = Qt::darkGreen; break;
        case 9: diskColor = QColor(255, 165, 0); break; // 橙色RGB值
        default: diskColor = Qt::lightGray; break;
    }
    
    QGraphicsRectItem* disk = m_gameScene->addRect(x, y, width, height, 
                                                  QPen(Qt::black, 1), 
                                                  QBrush(diskColor));
}

// 演示功能实现
void MainWindow::generateSolution()
{
    m_solution.clear();
    solveHanoi(m_diskCount, 0, 2, 1);  // 从柱子0移动到柱子2，使用柱子1作为辅助
}

void MainWindow::solveHanoi(int n, int from, int to, int aux)
{
    if (n == 1) {
        // 基础情况：只有一个盘子，直接移动
        m_solution.append(QPair<int, int>(from, to));
    } else {
        // 递归情况：
        // 1. 将上面n-1个盘子从from移动到aux
        solveHanoi(n - 1, from, aux, to);
        // 2. 将最大的盘子从from移动到to
        m_solution.append(QPair<int, int>(from, to));
        // 3. 将n-1个盘子从aux移动到to
        solveHanoi(n - 1, aux, to, from);
    }
}

void MainWindow::executeDemoStep()
{
    if (m_currentStep >= m_solution.size()) {
        // 演示完成
        m_demoTimer->stop();
        m_isDemonstrating = false;
        updateControlButtons();
        logMessage("演示完成！");
        
        // 检查是否获胜
        if (checkGameWon()) {
            onGameWon();
        }
        return;
    }
    
    // 执行当前步骤
    QPair<int, int> move = m_solution[m_currentStep];
    int from = move.first;
    int to = move.second;
    
    if (moveDiskBetweenTowers(from, to)) {
        logMessage(QString("演示步骤 %1/%2: 从柱子 %3 到柱子 %4")
                   .arg(m_currentStep + 1)
                   .arg(m_solution.size())
                   .arg(char('A' + from))
                   .arg(char('A' + to)));
        
        // 更新进度条
        int progress = ((m_currentStep + 1) * 100) / m_solution.size();
        m_progressBar->setValue(progress);
    }
    
    m_currentStep++;
} 
 