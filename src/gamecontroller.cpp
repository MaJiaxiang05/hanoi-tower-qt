#include "gamecontroller.h"
#include "hanoigame.h"
#include "gamedisplay.h"


GameController::GameController(QObject *parent)
    : QObject(parent)
    , m_game(nullptr)
    , m_display(nullptr)
    , m_demoTimer(nullptr)
    , m_currentStep(0)
    , m_isDemonstrating(false)
    , m_demoSpeed(5)
    , m_selectedTower(-1)
{
    m_demoTimer = new QTimer(this);
    connect(m_demoTimer, &QTimer::timeout, this, &GameController::executeDemoStep);
}

GameController::~GameController()
{
}

void GameController::setGame(HanoiGame* game)//设置游戏，连接HanoiGame::与GameController::
{
    m_game = game;
    if (m_game) {
        connect(m_game, &HanoiGame::gameWon, this, &GameController::onGameWon);
        connect(m_game, &HanoiGame::moveExecuted, this, &GameController::onMoveExecuted);
        connect(m_game, &HanoiGame::invalidMove, this, &GameController::onInvalidMove);
    }
}

void GameController::setDisplay(GameDisplay* display)
{
    m_display = display;
}

void GameController::startDemo()// 开始演示
{
    if (!m_game) {
        return;
    }
    
    emit logMessage("开始演示最优解");
    
    // 重置游戏状态
    m_game->resetGame();
    m_selectedTower = -1;
    updateDisplay();
    
    // 生成解决方案
    generateSolution();
    
    if (m_solution.isEmpty()) {
        emit logMessage("无法生成解决方案");
        return;
    }
    
    m_isDemonstrating = true;
    m_currentStep = 0;
    
    // 发出初始进度信号
    emit demoProgressUpdated(0, m_solution.size());
    
    // 开始演示，使用当前设置的速度
    int interval = 2000 - (m_demoSpeed - 1) * 200;  // 根据速度计算间隔
    if (interval < 100) interval = 100;  // 最小间隔100ms
    m_demoTimer->start(interval);
    
    emit demoStarted();
    emit logMessage(QString("开始演示，共%1步").arg(m_solution.size()));
}

void GameController::stopDemo()// 停止演示
{
    if (m_demoTimer) {
        m_demoTimer->stop();
    }
    m_isDemonstrating = false;
    m_currentStep = 0;
    m_solution.clear();
    
    emit demoStopped();
    emit logMessage("停止演示");
}

void GameController::pauseDemo()// 暂停演示
{
    if (!m_isDemonstrating) return;
    
    if (m_demoTimer->isActive()) {
        // 当前正在运行，暂停它
        m_demoTimer->stop();
        emit demoPaused();
        emit logMessage("暂停演示");
    } else {
        // 当前已暂停，恢复运行
        int interval = 2000  - (m_demoSpeed - 1) * 200;
        if (interval < 100) interval = 100;
        m_demoTimer->start(interval);
        emit demoResumed();
        emit logMessage("继续演示");
    }
}

void GameController::resetGame()
{
    // 如果正在演示，先停止演示
    if (m_isDemonstrating) {
        stopDemo();
    }
    
    // 重置游戏状态
    if (m_game) {
        m_game->resetGame();
    }
    
    m_selectedTower = -1;
    updateDisplay();
    
    emit gameReset();
    emit logMessage("重置游戏");
}

void GameController::setDiskCount(int count)
{
    if (m_game) {
        m_game->initializeGame(count);
        updateDisplay();
        emit logMessage(QString("设置盘子数量为: %1").arg(count));
    }
}

void GameController::setDemoSpeed(int speed)
{
    m_demoSpeed = speed;
    
    if (m_demoTimer && m_isDemonstrating) {
        int interval = 2000 - (speed - 1) * 200;  // 速度1=1800ms, 速度10=200ms
        if (interval < 100) interval = 100;  // 最小间隔100ms
        
        if (m_demoTimer->isActive()) {
            m_demoTimer->start(interval);  // 立即应用新的间隔
        }
    }
}

void GameController::selectTower(int towerIndex)
{
    if (m_isDemonstrating || !m_game) {
        return;
    }
    
    if (m_selectedTower == -1) {
        // 第一次选择
        QList<QList<int> > towers = m_game->getTowers();
        if (towerIndex >= 0 && towerIndex < towers.size() && !towers[towerIndex].isEmpty()) {
            m_selectedTower = towerIndex;
            emit logMessage(QString("选择了柱子 %1").arg(char('A' + towerIndex)));
        } else {
            emit logMessage("该柱子上没有盘子");
        }
    } else {
        // 第二次选择，尝试移动
        if (m_selectedTower != towerIndex) {
            m_game->moveDisk(m_selectedTower, towerIndex);
        }
        m_selectedTower = -1;
    }
    
    updateDisplay();
}

bool GameController::isDemonstrating() const
{
    return m_isDemonstrating;
}

bool GameController::isPaused() const
{
    return m_isDemonstrating && !m_demoTimer->isActive();
}

int GameController::getSelectedTower() const
{
    return m_selectedTower;
}

void GameController::executeDemoStep()
{
    if (m_currentStep >= m_solution.size()) {
        // 演示完成但没有触发游戏胜利，手动停止
        emit demoProgressUpdated(m_solution.size(), m_solution.size());
        QTimer::singleShot(200, this, [this]() {
            stopDemo();
            emit logMessage("演示完成！");
        });
        return;
    }
    
    QPair<int, int> move = m_solution[m_currentStep];
    if (m_game) {
        m_game->moveDisk(move.first, move.second);
    }
    
    m_currentStep++;
    emit demoProgressUpdated(m_currentStep, m_solution.size());
    
    emit logMessage(QString("第%1步: 从柱子%2移动到柱子%3")
                   .arg(m_currentStep)
                   .arg(char('A' + move.first))
                   .arg(char('A' + move.second)));
}

void GameController::onGameWon()
{
    if (m_isDemonstrating) {
        // 确保进度条达到100%
        emit demoProgressUpdated(m_solution.size(), m_solution.size());
        // 延迟停止演示，让用户看到完成状态
        QTimer::singleShot(500, this, [this]() {
            stopDemo();
        });
    }
    emit gameWon();
    emit logMessage("恭喜！您完成了汉诺塔！");
}

void GameController::onMoveExecuted(int from, int to, int diskSize)
{
    updateDisplay();
    if (!m_isDemonstrating) {
        emit logMessage(QString("移动盘子%1: 从柱子%2到柱子%3")
                       .arg(diskSize)
                       .arg(char('A' + from))
                       .arg(char('A' + to)));
    }
}

void GameController::onInvalidMove(const QString &reason)
{
    emit invalidMove(reason);
    emit logMessage("无效移动: " + reason);
}

void GameController::updateDisplay()
{
    if (m_display && m_game) {
        m_display->drawGame(m_game->getTowers(), m_selectedTower);
    }
}

void GameController::generateSolution()
{
    m_solution.clear();
    if (!m_game) return;
    
    // 直接使用HanoiGame类的generateSolution方法
    m_solution = m_game->generateSolution();
}

 
