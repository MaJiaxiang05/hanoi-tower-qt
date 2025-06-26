#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include <QObject>
#include <QTimer>
#include <QPair>
#include <QList>
#include <QDebug>
#include <QTimer>

class HanoiGame;
class GameDisplay;

class GameController : public QObject
{
    Q_OBJECT

public:
    explicit GameController(QObject *parent = nullptr);
    ~GameController();

    // 设置组件
    void setGame(HanoiGame* game);
    void setDisplay(GameDisplay* display);

    // 游戏控制
    void startDemo();
    void stopDemo();
    void pauseDemo();
    void resetGame();
    void setDiskCount(int count);
    void setDemoSpeed(int speed);

    // 手动操作
    void selectTower(int towerIndex);

    // 获取状态
    bool isDemonstrating() const;
    bool isPaused() const;
    int getSelectedTower() const;

signals:
    void demoStarted();
    void demoStopped();
    void demoPaused();
    void demoResumed();
    void gameReset();
    void gameWon();
    void invalidMove(const QString &message);
    void logMessage(const QString &message);
    void demoProgressUpdated(int currentStep, int totalSteps);

private slots:
    void executeDemoStep();
    void onGameWon();
    void onMoveExecuted(int from, int to, int diskSize);
    void onInvalidMove(const QString &reason);

private:
    void updateDisplay();
    void generateSolution();

private:
    HanoiGame* m_game;
    GameDisplay* m_display;

    // 演示相关
    QTimer* m_demoTimer;
    QList<QPair<int, int> > m_solution;
    int m_currentStep;
    bool m_isDemonstrating;
    int m_demoSpeed;

    // 手动操作
    int m_selectedTower;
};

#endif // GAMECONTROLLER_H 
