#ifndef MAINWINDOW_MULTI_H
#define MAINWINDOW_MULTI_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QPushButton>
#include <QSpinBox>
#include <QSlider>//速度条
#include <QProgressBar>//演示进度条
#include <QTextEdit>
#include <QLabel>
#include <QGroupBox>//控件
#include <QSplitter>// 控件
#include <QMessageBox>//显示消息框
#include <QTime>

// 前向声明
class HanoiGame;
class GameDisplay;
class GameController;
class QAction;//用户界面

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // 游戏控制槽函数
    void onStartDemo();
    void onStopDemo();
    void onPauseDemo();
    void onResetGame();
    void onDiskCountChanged(int count);
    void onSpeedChanged(int speed);
    
    // 手动操作槽函数
    void onTowerAClicked();
    void onTowerBClicked();
    void onTowerCClicked();
    
    // 游戏事件槽函数
    void onGameWon();
    void onLogMessage(const QString &message);
    void onInvalidMove(const QString &message);
    void onDemoProgressUpdated(int currentStep, int totalSteps);
    void onMoveExecutedDebug(int from, int to, int diskSize);
    
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
    
    QWidget* createControlWidget();
    void updateControlButtons();
    void updateGameInfo();
    void showGameWonDialog();

private:
    // 核心组件
    HanoiGame* m_game;
    GameDisplay* m_display;
    GameController* m_controller;
    
    // UI组件
    QWidget* m_centralWidget;
    QSplitter* m_mainSplitter;
    
    // 游戏显示区域
    QGraphicsView* m_gameView;
    
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
};

#endif // MAINWINDOW_MULTI_H 
