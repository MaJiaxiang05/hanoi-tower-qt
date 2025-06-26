#include "mainwindow.h"
#include <QMessageBox>
#include <QApplication>
#include <QDateTime>
#include <QGridLayout>
#include <QMenuBar>
#include <QStatusBar>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_game(nullptr)
    , m_controller(nullptr)
    , m_selectedTower(-1)
    , m_isDemonstrating(false)
{
    setWindowTitle("汉诺塔游戏");
    setMinimumSize(1000, 700);
    
    // 创建游戏和控制器
    m_game = new HanoiTower(3, this);
    m_controller = new GameController(this);
    m_controller->setGame(m_game);
    
    setupUI();
    setupConnections();
    updateGameDisplay();
    updateControlButtons();
    
    logMessage("欢迎来到汉诺塔游戏！");
}

MainWindow::~MainWindow()
{
    // Qt的父子关系会自动删除子对象
}

void MainWindow::setupUI()
{
    setupMenuBar();
    setupGameArea();
    setupControlPanel();
    setupStatusBar();
    
    // 主布局
    m_centralWidget = new QWidget;
    setCentralWidget(m_centralWidget);
    
    m_mainSplitter = new QSplitter(Qt::Horizontal, this);
    
    // 左侧：游戏显示区域
    m_mainSplitter->addWidget(m_gameView);
    
    // 右侧：控制面板
    QWidget* rightPanel = new QWidget;
    QVBoxLayout* rightLayout = new QVBoxLayout(rightPanel);
    rightLayout->addWidget(m_controlGroup);
    rightLayout->addWidget(m_settingsGroup);
    rightLayout->addWidget(m_manualGroup);
    rightLayout->addWidget(m_infoGroup);
    rightLayout->addWidget(m_logGroup);
    rightLayout->addStretch();
    
    m_mainSplitter->addWidget(rightPanel);
    m_mainSplitter->setStretchFactor(0, 3);  // 游戏区域占更多空间
    m_mainSplitter->setStretchFactor(1, 1);
    
    QHBoxLayout* mainLayout = new QHBoxLayout(m_centralWidget);
    mainLayout->addWidget(m_mainSplitter);
}

void MainWindow::setupMenuBar()
{
    // 文件菜单
    QMenu* fileMenu = menuBar()->addMenu("文件(&F)");
    
    m_exitAction = new QAction("退出(&X)", this);
    m_exitAction->setShortcut(QKeySequence::Quit);
    m_exitAction->setStatusTip("退出游戏");
    connect(m_exitAction, &QAction::triggered, this, &QWidget::close);
    fileMenu->addAction(m_exitAction);
    
    // 帮助菜单
    QMenu* helpMenu = menuBar()->addMenu("帮助(&H)");
    
    m_howToPlayAction = new QAction("游戏规则(&R)", this);
    m_howToPlayAction->setStatusTip("查看游戏规则");
    connect(m_howToPlayAction, &QAction::triggered, this, &MainWindow::onHowToPlay);
    helpMenu->addAction(m_howToPlayAction);
    
    helpMenu->addSeparator();
    
    m_aboutAction = new QAction("关于(&A)", this);
    m_aboutAction->setStatusTip("关于汉诺塔游戏");
    connect(m_aboutAction, &QAction::triggered, this, &MainWindow::onAbout);
    helpMenu->addAction(m_aboutAction);
}

void MainWindow::setupGameArea()
{
    // 创建游戏场景
    m_gameScene = new QGraphicsScene(this);
    m_gameScene->setSceneRect(0, 0, SCENE_WIDTH, SCENE_HEIGHT);
    m_gameScene->setBackgroundBrush(QBrush(QColor(240, 248, 255)));  // 浅蓝色背景
    
    // 添加柱子到场景
    QList<Tower*> towers = m_game->getTowers();
    for (int i = 0; i < towers.size(); ++i) {
        Tower* tower = towers[i];
        tower->setPos(150 + i * TOWER_SPACING, SCENE_HEIGHT - 100);
        m_gameScene->addItem(tower);
    }
    
    // 创建游戏视图
    m_gameView = new QGraphicsView(m_gameScene, this);
    m_gameView->setRenderHint(QPainter::Antialiasing);
    m_gameView->setFixedSize(SCENE_WIDTH + 20, SCENE_HEIGHT + 20);
}

void MainWindow::setupControlPanel()
{
    // 演示控制组
    m_controlGroup = new QGroupBox("演示控制");
    QVBoxLayout* controlLayout = new QVBoxLayout(m_controlGroup);
    
    m_startDemoButton = new QPushButton("开始演示");
    m_pauseButton = new QPushButton("暂停");
    m_stopButton = new QPushButton("停止");
    m_resetButton = new QPushButton("重置游戏");
    
    m_pauseButton->setEnabled(false);
    m_stopButton->setEnabled(false);
    
    controlLayout->addWidget(m_startDemoButton);
    controlLayout->addWidget(m_pauseButton);
    controlLayout->addWidget(m_stopButton);
    controlLayout->addWidget(m_resetButton);
    
    // 设置组
    m_settingsGroup = new QGroupBox("游戏设置");
    QGridLayout* settingsLayout = new QGridLayout(m_settingsGroup);
    
    settingsLayout->addWidget(new QLabel("盘子数量:"), 0, 0);
    m_diskCountSpinBox = new QSpinBox;
    m_diskCountSpinBox->setRange(1, 10);
    m_diskCountSpinBox->setValue(3);
    settingsLayout->addWidget(m_diskCountSpinBox, 0, 1);
    
    settingsLayout->addWidget(new QLabel("演示速度:"), 1, 0);
    m_speedSlider = new QSlider(Qt::Horizontal);
    m_speedSlider->setRange(1, 10);
    m_speedSlider->setValue(5);
    settingsLayout->addWidget(m_speedSlider, 1, 1);
    
    m_speedLabel = new QLabel("中等");
    settingsLayout->addWidget(m_speedLabel, 1, 2);
    
    // 手动操作组
    m_manualGroup = new QGroupBox("手动操作");
    QHBoxLayout* manualLayout = new QHBoxLayout(m_manualGroup);
    
    m_towerAButton = new QPushButton("柱子 A");
    m_towerBButton = new QPushButton("柱子 B");
    m_towerCButton = new QPushButton("柱子 C");
    
    manualLayout->addWidget(m_towerAButton);
    manualLayout->addWidget(m_towerBButton);
    manualLayout->addWidget(m_towerCButton);
    
    // 信息面板
    m_infoGroup = new QGroupBox("游戏信息");
    QGridLayout* infoLayout = new QGridLayout(m_infoGroup);
    
    infoLayout->addWidget(new QLabel("当前步数:"), 0, 0);
    m_moveCountLabel = new QLabel("0");
    infoLayout->addWidget(m_moveCountLabel, 0, 1);
    
    infoLayout->addWidget(new QLabel("最优步数:"), 1, 0);
    m_optimalMovesLabel = new QLabel("7");
    infoLayout->addWidget(m_optimalMovesLabel, 1, 1);
    
    m_progressBar = new QProgressBar;
    m_progressBar->setVisible(false);  // 初始时隐藏
    infoLayout->addWidget(m_progressBar, 2, 0, 1, 2);
    
    // 日志面板
    m_logGroup = new QGroupBox("操作日志");
    QVBoxLayout* logLayout = new QVBoxLayout(m_logGroup);
    
    m_logTextEdit = new QTextEdit;
    m_logTextEdit->setMaximumHeight(120);
    m_logTextEdit->setReadOnly(true);
    logLayout->addWidget(m_logTextEdit);
}

void MainWindow::setupStatusBar()
{
    statusBar()->showMessage("准备就绪");
}

void MainWindow::setupConnections()
{
    // 控制按钮连接
    connect(m_startDemoButton, &QPushButton::clicked, this, &MainWindow::onStartDemonstration);
    connect(m_pauseButton, &QPushButton::clicked, this, &MainWindow::onPauseDemonstration);
    connect(m_stopButton, &QPushButton::clicked, this, &MainWindow::onStopDemonstration);
    connect(m_resetButton, &QPushButton::clicked, this, &MainWindow::onResetGame);
    
    // 设置连接
    connect(m_diskCountSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), 
            this, &MainWindow::onDiskCountChanged);
    connect(m_speedSlider, &QSlider::valueChanged, this, &MainWindow::onSpeedChanged);
    
    // 手动操作连接
    connect(m_towerAButton, &QPushButton::clicked, this, &MainWindow::onTowerAClicked);
    connect(m_towerBButton, &QPushButton::clicked, this, &MainWindow::onTowerBClicked);
    connect(m_towerCButton, &QPushButton::clicked, this, &MainWindow::onTowerCClicked);
    
    // 游戏状态连接
    connect(m_game, &HanoiTower::gameWon, this, &MainWindow::onGameWon);
    connect(m_game, &HanoiTower::moveCountChanged, this, &MainWindow::onMoveCountChanged);
    connect(m_game, &HanoiTower::invalidMoveAttempted, this, &MainWindow::onInvalidMoveAttempted);
    
    // 控制器连接
        connect(m_controller, &GameController::demoProgressUpdated,
            this, &MainWindow::onDemonstrationStepExecuted);
    connect(m_controller, &GameController::demoStopped,
            this, &MainWindow::onDemonstrationFinished);
}

void MainWindow::onStartDemonstration()
{
    m_controller->startDemo();
    m_isDemonstrating = true;
    updateControlButtons();
    
    m_progressBar->setVisible(true);
    m_progressBar->setMaximum(m_game->getOptimalMoves());
    m_progressBar->setValue(0);
    
    logMessage("开始演示最优解法...");
    statusBar()->showMessage("正在演示最优解法");
}

void MainWindow::onStopDemonstration()
{
    m_controller->stopDemo();
    m_isDemonstrating = false;
    updateControlButtons();
    
    m_progressBar->setVisible(false);
    
    logMessage("演示已停止");
    statusBar()->showMessage("演示已停止");
}

void MainWindow::onPauseDemonstration()
{
    if (m_controller->isPaused()) {
        m_controller->pauseDemo();  // pauseDemo会恢复
        m_pauseButton->setText("暂停");
        logMessage("演示已恢复");
        statusBar()->showMessage("演示已恢复");
    } else {
        m_controller->pauseDemo();
        m_pauseButton->setText("继续");
        logMessage("演示已暂停");
        statusBar()->showMessage("演示已暂停");
    }
}

void MainWindow::onResetGame()
{
    m_controller->resetGame();
    m_selectedTower = -1;
    updateControlButtons();
    updateGameDisplay();
    
    m_progressBar->setVisible(false);
    
    logMessage("游戏已重置");
    statusBar()->showMessage("游戏已重置");
}

void MainWindow::onDiskCountChanged(int count)
{
    if (!m_isDemonstrating) {
        m_controller->setDiskCount(count);
        m_optimalMovesLabel->setText(QString::number(m_game->getOptimalMoves()));
        updateGameDisplay();
        logMessage(QString("盘子数量设置为: %1").arg(count));
    }
}

void MainWindow::onSpeedChanged(int speed)
{
    m_controller->setDemoSpeed(speed);
    
    QString speedText;
    if (speed <= 3) speedText = "慢";
    else if (speed <= 7) speedText = "中等";
    else speedText = "快";
    
    m_speedLabel->setText(speedText);
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

void MainWindow::onGameWon()
{
    logMessage("恭喜！游戏完成！");
    showGameWonDialog();
}

void MainWindow::onMoveCountChanged(int count)
{
    m_moveCountLabel->setText(QString::number(count));
    if (m_progressBar->isVisible()) {
        m_progressBar->setValue(count);
    }
}

void MainWindow::onDemonstrationStepExecuted(int step, int from, int to)
{
    QString fromTower = QString(QChar('A' + from));
    QString toTower = QString(QChar('A' + to));
    logMessage(QString("步骤 %1: 从柱子 %2 移动到柱子 %3").arg(step + 1).arg(fromTower).arg(toTower));
}

void MainWindow::onDemonstrationFinished()
{
    m_isDemonstrating = false;
    updateControlButtons();
    
    m_progressBar->setVisible(false);
    
    logMessage("演示完成！");
    statusBar()->showMessage("演示完成");
    
    QMessageBox::information(this, "演示完成", 
                           QString("演示完成！\n"
                                  "总共用了 %1 步，这是最优解法。")
                           .arg(m_game->getMoveCount()));
}

void MainWindow::onInvalidMoveAttempted(const QString &message)
{
    logMessage(QString("无效移动: %1").arg(message));
    statusBar()->showMessage(message, 3000);
}

void MainWindow::onAbout()
{
    QMessageBox::about(this, "关于汉诺塔游戏",
                      "汉诺塔游戏 v1.0\n\n"
                      "这是一个经典的汉诺塔益智游戏。\n"
                      "目标是将所有盘子从第一根柱子移动到最后一根柱子。\n\n"
                      "规则：\n"
                      "1. 一次只能移动一个盘子\n"
                      "2. 只能移动柱子顶部的盘子\n"
                      "3. 大盘子不能放在小盘子上面\n\n"
                      "使用Qt开发 © 2024");
}

void MainWindow::onHowToPlay()
{
    QMessageBox::information(this, "游戏规则",
                           "汉诺塔游戏规则：\n\n"
                           "目标：\n"
                           "将所有盘子从柱子A移动到柱子C\n\n"
                           "规则：\n"
                           "1. 一次只能移动一个盘子\n"
                           "2. 只能移动柱子顶部的盘子\n"
                           "3. 大盘子不能放在小盘子上面\n\n"
                           "操作方法：\n"
                           "• 手动模式：先点击源柱子，再点击目标柱子\n"
                           "• 演示模式：点击"开始演示"查看最优解法\n"
                           "• 可以调整盘子数量和演示速度");
}

void MainWindow::updateGameDisplay()
{
    // 游戏显示由Qt的graphics view自动更新
    m_gameScene->update();
}

void MainWindow::updateControlButtons()
{
    bool demonstrating = m_controller->isDemonstrating();
    
    m_startDemoButton->setEnabled(!demonstrating);
    m_pauseButton->setEnabled(demonstrating);
    m_stopButton->setEnabled(demonstrating);
    
    // 演示时禁用手动操作和设置
    m_manualGroup->setEnabled(!demonstrating);
    m_diskCountSpinBox->setEnabled(!demonstrating);
    
    // 更新柱子按钮状态
    if (!demonstrating) {
        m_towerAButton->setStyleSheet(m_selectedTower == 0 ? "background-color: lightblue;" : "");
        m_towerBButton->setStyleSheet(m_selectedTower == 1 ? "background-color: lightblue;" : "");
        m_towerCButton->setStyleSheet(m_selectedTower == 2 ? "background-color: lightblue;" : "");
    }
}

void MainWindow::logMessage(const QString &message)
{
    QString timestamp = QDateTime::currentDateTime().toString("hh:mm:ss");
    m_logTextEdit->append(QString("[%1] %2").arg(timestamp).arg(message));
    
    // 滚动到底部
    QTextCursor cursor = m_logTextEdit->textCursor();
    cursor.movePosition(QTextCursor::End);
    m_logTextEdit->setTextCursor(cursor);
}

void MainWindow::showGameWonDialog()
{
    int moves = m_game->getMoveCount();
    int optimal = m_game->getOptimalMoves();
    
    QString message = QString("游戏完成！\n\n"
                             "您用了 %1 步\n"
                             "最优步数是 %2 步\n")
                     .arg(moves).arg(optimal);
    
    if (moves == optimal) {
        message += "\n完美！您达到了最优解！";
    } else {
        message += QString("\n还有改进空间，再试一次吧！");
    }
    
    QMessageBox::information(this, "游戏完成", message);
}

void MainWindow::selectTower(int towerIndex)
{
    if (m_isDemonstrating) {
        return;
    }
    
    // 直接使用GameController的selectTower方法
    m_controller->selectTower(towerIndex);
    
    // 更新本地状态
    m_selectedTower = m_controller->getSelectedTower();
    
    updateControlButtons();
}

void MainWindow::attemptMove()
{
    if (m_selectedTower == -1) {
        return;
    }
    
    // 获取目标柱子（当前点击的柱子）
    int targetTower = -1;
    if (sender() == m_towerAButton) targetTower = 0;
    else if (sender() == m_towerBButton) targetTower = 1;
    else if (sender() == m_towerCButton) targetTower = 2;
    
    if (targetTower != -1) {
        bool success = m_controller->attemptMove(m_selectedTower, targetTower);
        
        QString sourceName = QString(QChar('A' + m_selectedTower));
        QString targetName = QString(QChar('A' + targetTower));
        
        if (success) {
            logMessage(QString("成功移动：从柱子 %1 到柱子 %2").arg(sourceName).arg(targetName));
        } else {
            logMessage(QString("移动失败：从柱子 %1 到柱子 %2").arg(sourceName).arg(targetName));
        }
    }
} 