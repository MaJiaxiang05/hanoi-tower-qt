#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QSpinBox>
#include <QLabel>
#include <QSlider>
#include <QProgressBar>
#include <QStatusBar>
#include <QMenuBar>
#include <QAction>
#include <QGroupBox>
#include <QTextEdit>
#include <QSplitter>

#include "hanoitower.h"
#include "gamecontroller.h"

QT_BEGIN_NAMESPACE
class QAction;
class QMenu;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // 游戏控制槽函数
    void onStartDemonstration();
    void onStopDemonstration();
    void onPauseDemonstration();
    void onResetGame();
    void onDiskCountChanged(int count);
    void onSpeedChanged(int speed);
    
    // 手动操作槽函数
    void onTowerAClicked();
    void onTowerBClicked();
    void onTowerCClicked();
    
    // 游戏状态槽函数
    void onGameWon();
    void onMoveCountChanged(int count);
    void onDemonstrationStepExecuted(int step, int from, int to);
    void onDemonstrationFinished();
    void onInvalidMoveAttempted(const QString &message);
    
    // 菜单槽函数
    void onAbout();
    void onHowToPlay();

private:
    void setupUI();
    void setupMenuBar();
    void setupGameArea();
    void setupControlPanel();
    void setupStatusBar();
    void setupConnections();
    
    void updateGameDisplay();
    void updateControlButtons();
    void logMessage(const QString &message);
    void showGameWonDialog();
    
    void selectTower(int towerIndex);
    void attemptMove();

private:
    // 核心组件
    HanoiTower* m_game;
    GameController* m_controller;
    
    // UI组件
    QWidget* m_centralWidget;
    QSplitter* m_mainSplitter;
    
    // 游戏显示区域
    QGraphicsView* m_gameView;
    QGraphicsScene* m_gameScene;
    
    // 控制面板
    QGroupBox* m_controlGroup;
    QPushButton* m_startDemoButton;
    QPushButton* m_pauseButton;
    QPushButton* m_stopButton;
    QPushButton* m_resetButton;
    
    QGroupBox* m_settingsGroup;
    QSpinBox* m_diskCountSpinBox;
    QSlider* m_speedSlider;
    QLabel* m_speedLabel;
    
    QGroupBox* m_manualGroup;
    QPushButton* m_towerAButton;
    QPushButton* m_towerBButton;
    QPushButton* m_towerCButton;
    
    // 信息面板
    QGroupBox* m_infoGroup;
    QLabel* m_moveCountLabel;
    QLabel* m_optimalMovesLabel;
    QProgressBar* m_progressBar;
    
    // 日志面板
    QGroupBox* m_logGroup;
    QTextEdit* m_logTextEdit;
    
    // 菜单和状态栏
    QAction* m_aboutAction;
    QAction* m_howToPlayAction;
    QAction* m_exitAction;
    
    // 游戏状态
    int m_selectedTower;  // 当前选中的柱子 (-1表示未选中)
    bool m_isDemonstrating;
    
    // 常量
    static const int SCENE_WIDTH = 800;
    static const int SCENE_HEIGHT = 400;
    static const int TOWER_SPACING = 250;
};

#endif // MAINWINDOW_H 