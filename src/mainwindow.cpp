#include "mainwindow.h"
#include "hanoigame.h"
#include "gamedisplay.h"
#include "gamecontroller.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMenuBar>
#include <QStatusBar>
#include <QAction>
#include <QDebug>
#include <QTime>
#include <QMessageBox>
#include <QThread>//控制进程，线程

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_game(NULL)
    , m_display(NULL)
    , m_controller(NULL)
    , m_centralWidget(NULL)
    , m_mainSplitter(NULL)
    , m_gameView(NULL)
{
    // 创建核心组件
    m_game = new HanoiGame(this);
    m_display = new GameDisplay(this);
    m_controller = new GameController(this);
    
    // 设置组件关系
    m_controller->setGame(m_game);
    m_controller->setDisplay(m_display);
    
    // 初始化游戏
    m_game->initializeGame(3);
    
    // 设置UI界面
    setupUI();
    setupMenuBar();
    setupStatusBar();
    setupConnections();
    
    // 初始更新
    updateControlButtons();
    updateGameInfo();
    
    setWindowTitle("汉诺塔游戏");
    resize(1000, 700);
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupUI()
{
    m_centralWidget = new QWidget;
    setCentralWidget(m_centralWidget);
    
    m_mainSplitter = new QSplitter(Qt::Horizontal);
    
    // 设置游戏显示区域
    setupGameArea();
    
    // 设置控制面板
    setupControlPanel();
    
    // 添加到分割器
    m_mainSplitter->addWidget(m_gameView);
    m_mainSplitter->addWidget(createControlWidget());
    m_mainSplitter->setStretchFactor(0, 3);  // 游戏区域占3/4
    m_mainSplitter->setStretchFactor(1, 1);  // 控制面板占1/4
    
    // 主布局
    QVBoxLayout* mainLayout = new QVBoxLayout(m_centralWidget);
    mainLayout->addWidget(m_mainSplitter);
}

void MainWindow::setupGameArea()
{
    m_gameView = new QGraphicsView;
    m_gameView->setMinimumSize(600, 400);
    
    // 将显示组件的场景设置到视图
    m_display->setGraphicsView(m_gameView);
    
    // 绘制初始游戏状态
    m_display->drawGame(m_game->getTowers());
}

void MainWindow::setupControlPanel()
{
    // 控制按钮组
    m_controlGroup = new QGroupBox("游戏控制");
    QVBoxLayout* controlLayout = new QVBoxLayout(m_controlGroup);
    
    m_startDemoButton = new QPushButton("开始演示");
    m_pauseButton = new QPushButton("暂停");
    m_stopButton = new QPushButton("停止演示");
    m_resetButton = new QPushButton("重置游戏");
    
    controlLayout->addWidget(m_startDemoButton);
    controlLayout->addWidget(m_pauseButton);
    controlLayout->addWidget(m_stopButton);
    controlLayout->addWidget(m_resetButton);
    
    // 设置组
    m_settingsGroup = new QGroupBox("游戏设置");
    QVBoxLayout* settingsLayout = new QVBoxLayout(m_settingsGroup);
    
    // 盘子数量设置
    settingsLayout->addWidget(new QLabel("盘子数量:"));
    m_diskCountSpinBox = new QSpinBox;
    m_diskCountSpinBox->setRange(1, 10);
    m_diskCountSpinBox->setValue(3);
    settingsLayout->addWidget(m_diskCountSpinBox);
    
    // 演示速度设置
    settingsLayout->addWidget(new QLabel("演示速度:"));
    m_speedSlider = new QSlider(Qt::Horizontal);
    m_speedSlider->setRange(1, 10);
    m_speedSlider->setValue(5);
    m_speedLabel = new QLabel("5");
    
    QHBoxLayout* speedLayout = new QHBoxLayout;
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
}

QWidget* MainWindow::createControlWidget()
{
    QWidget* controlWidget = new QWidget;
    controlWidget->setMinimumWidth(250);
    controlWidget->setMaximumWidth(300);
    
    QVBoxLayout* layout = new QVBoxLayout(controlWidget);
    
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
    
    // 连接菜单信号槽
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
    // 连接控制按钮信号槽
    connect(m_startDemoButton, SIGNAL(clicked()), this, SLOT(onStartDemo()));
    connect(m_pauseButton, SIGNAL(clicked()), this, SLOT(onPauseDemo()));
    connect(m_stopButton, SIGNAL(clicked()), this, SLOT(onStopDemo()));
    connect(m_resetButton, SIGNAL(clicked()), this, SLOT(onResetGame()));
    
    // 连接设置控件信号槽
    connect(m_diskCountSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onDiskCountChanged(int)));
    connect(m_speedSlider, SIGNAL(valueChanged(int)), this, SLOT(onSpeedChanged(int)));
    
    // 连接手动操作按钮
    connect(m_towerAButton, SIGNAL(clicked()), this, SLOT(onTowerAClicked()));
    connect(m_towerBButton, SIGNAL(clicked()), this, SLOT(onTowerBClicked()));
    connect(m_towerCButton, SIGNAL(clicked()), this, SLOT(onTowerCClicked()));
    
    // 连接游戏控制器信号
    connect(m_controller, SIGNAL(gameWon()), this, SLOT(onGameWon()));
    connect(m_controller, SIGNAL(logMessage(QString)), this, SLOT(onLogMessage(QString)));
    connect(m_controller, SIGNAL(invalidMove(QString)), this, SLOT(onInvalidMove(QString)));
    connect(m_controller, SIGNAL(demoStarted()), this, SLOT(updateControlButtons()));
    connect(m_controller, SIGNAL(demoStopped()), this, SLOT(updateControlButtons()));
    connect(m_controller, SIGNAL(demoPaused()), this, SLOT(updateControlButtons()));
    connect(m_controller, SIGNAL(demoResumed()), this, SLOT(updateControlButtons()));
    connect(m_controller, SIGNAL(demoProgressUpdated(int,int)), this, SLOT(onDemoProgressUpdated(int,int)));
    
    // 连接游戏状态变化信号
    connect(m_game, &HanoiGame::moveExecuted, this, &MainWindow::updateGameInfo);
    connect(m_controller, &GameController::gameReset, this, &MainWindow::updateGameInfo);
    // 确保每次演示步骤后也更新游戏信息
    connect(m_controller, QOverload<int,int>::of(&GameController::demoProgressUpdated), 
            this, &MainWindow::updateGameInfo);
    // 额外连接：确保每次移动都能看到调试信息
    connect(m_game, &HanoiGame::moveExecuted, this, &MainWindow::onMoveExecutedDebug);
}

// 槽函数实现
void MainWindow::onStartDemo()
{
    m_controller->startDemo();
    updateControlButtons();
    updateGameInfo();
}

void MainWindow::onStopDemo()
{
    m_controller->stopDemo();
    m_progressBar->setValue(0);  // 清空进度条
    updateControlButtons();
    updateGameInfo();  // 更新游戏信息
}

void MainWindow::onPauseDemo()
{
    m_controller->pauseDemo();
    updateControlButtons();
    updateGameInfo();  // 确保状态信息正确
}

void MainWindow::onResetGame()
{
    m_controller->resetGame();
    m_progressBar->setValue(0);  // 清空进度条
    updateGameInfo();
    updateControlButtons();
}

void MainWindow::onDiskCountChanged(int count)
{
    m_controller->setDiskCount(count);
    updateGameInfo();
}

void MainWindow::onSpeedChanged(int speed)
{
    m_speedLabel->setText(QString::number(speed));
    m_controller->setDemoSpeed(speed);
}

void MainWindow::onTowerAClicked()
{
    m_controller->selectTower(0);
    updateGameInfo();  // 手动操作后更新游戏信息
}

void MainWindow::onTowerBClicked()
{
    m_controller->selectTower(1);
    updateGameInfo();  // 手动操作后更新游戏信息
}

void MainWindow::onTowerCClicked()
{
    m_controller->selectTower(2);
    updateGameInfo();  // 手动操作后更新游戏信息
}

void MainWindow::onGameWon()
{
    updateControlButtons();  // 游戏胜利后更新按钮状态
    updateGameInfo();        // 更新最终的游戏信息
    showGameWonDialog();
}

void MainWindow::onLogMessage(const QString &message)
{
    if (m_logTextEdit) {
        m_logTextEdit->append(QString("[%1] %2")
                             .arg(QTime::currentTime().toString("hh:mm:ss"))
                             .arg(message));
    }
}

void MainWindow::onInvalidMove(const QString &message)
{
    onLogMessage(QString("错误: %1").arg(message));
}

void MainWindow::onDemoProgressUpdated(int currentStep, int totalSteps)
{
    if (totalSteps > 0) {
        int progress = (currentStep * 100) / totalSteps;
        m_progressBar->setValue(progress);
    }
}

void MainWindow::onMoveExecutedDebug(int from, int to, int diskSize)
{
    Q_UNUSED(from)
    Q_UNUSED(to)
    Q_UNUSED(diskSize)
    
    // 这个函数主要用于调试，确保信号连接正常工作，所以函数虽然传进来了参数，但函数体内没用到，因为删除了Debug
    // 强制更新游戏信息
    updateGameInfo();
}

void MainWindow::onAbout()
{
    QMessageBox::about(this, "关于汉诺塔游戏",
                      "汉诺塔游戏 v1.0 \n\n"
                      "这是一个经典的递归算法演示程序。\n"
                      "目标是将所有盘子从左边移动到右边，\n"
                      "遵循以下规则：\n"
                      "1. 一次只能移动一个盘子\n"
                      "2. 只能移动柱子顶部的盘子\n"
                      "3. 大盘子不能放在小盘子上面\n\n"
                      "架构说明：\n"
                      "- HanoiGame: 游戏逻辑\n"
                      "- GameDisplay: 图形绘制\n"
                      "- GameController: 游戏控制\n"
                      "- MainWindow: 用户界面");
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

void MainWindow::updateControlButtons()
{
    bool demonstrating = m_controller->isDemonstrating();
    bool paused = m_controller->isPaused();
    
    // 开始按钮：只有在不演示时才能点击
    m_startDemoButton->setEnabled(!demonstrating);
    
    // 暂停按钮：只有在演示时才能点击，根据暂停状态显示不同文本
    m_pauseButton->setEnabled(demonstrating);
    if (demonstrating) {
        m_pauseButton->setText(paused ? "继续" : "暂停");
    } else {
        m_pauseButton->setText("暂停");
    }
    
    // 停止按钮：只有在演示时才能点击
    m_stopButton->setEnabled(demonstrating);
    
    // 重置按钮：始终可用（但演示时会先停止演示）
    m_resetButton->setEnabled(true);
    
    // 设置控件：演示时禁用
    m_diskCountSpinBox->setEnabled(!demonstrating);
    m_speedSlider->setEnabled(true);  // 速度滑块在演示时也可以调整
    
    // 手动操作按钮：演示时禁用
    m_towerAButton->setEnabled(!demonstrating);
    m_towerBButton->setEnabled(!demonstrating);
    m_towerCButton->setEnabled(!demonstrating);
}

void MainWindow::updateGameInfo()
{
    if (!m_game) {
        return;
    }
    
    int moveCount = m_game->getMoveCount();
    int optimalMoves = m_game->getOptimalMoves();
    
    QString newMoveText = QString("移动步数: %1").arg(moveCount);
    QString newOptimalText = QString("最优步数: %1").arg(optimalMoves);
    
    m_moveCountLabel->setText(newMoveText);
    m_optimalMovesLabel->setText(newOptimalText);
}

void MainWindow::showGameWonDialog()
{
    int moveCount = m_game->getMoveCount();
    int optimal = m_game->getOptimalMoves();
    int efficiency = (optimal * 100) / moveCount;
    
    QMessageBox::information(this, "恭喜!", 
                           QString("游戏完成!\n用了 %1 步\n最优步数: %2\n效率: %3%")
                           .arg(moveCount)
                           .arg(optimal)
                           .arg(efficiency));
}

 
