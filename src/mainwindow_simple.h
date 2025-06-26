#ifndef MAINWINDOW_SIMPLE_H
#define MAINWINDOW_SIMPLE_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QPushButton>
#include <QSpinBox>
#include <QLabel>
#include <QSlider>
#include <QProgressBar>
#include <QTextEdit>
#include <QGroupBox>
#include <QSplitter>
#include <QList>
#include <QPair>
#include <QTimer>

// Qt 5.0.2兼容：暂时注释掉游戏逻辑类
// class HanoiTower;
// class GameController;
class QAction;

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
    
    // 演示槽函数
    void executeDemoStep();

private:
    void setupUI();
    void setupMenuBar();
    void setupGameArea();
    void setupStatusBar();
    void setupConnections();
    
    QWidget* createControlWidget();
    
    void updateGameDisplay();
    void updateControlButtons();
    void logMessage(const QString &message);
    void showGameWonDialog();
    
    void selectTower(int towerIndex);
    bool checkGameWon();
    
    // 图形绘制函数
    void drawTowersAndDisks();
    void drawTower(int towerIndex, int x, int y);
    void drawDisk(int size, int towerIndex, int position);
    
    // 游戏逻辑函数
    void initializeGame();
    void resetGameData();
    bool moveDiskBetweenTowers(int from, int to);
    
    // 演示功能
    void generateSolution();
    void solveHanoi(int n, int from, int to, int aux);

private:
    // Qt 5.0.2兼容：暂时使用void*替代具体类型
    void* m_game;
    void* m_controller;
    
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
    
    // 简单的游戏数据（Qt 5.0.2兼容版本）
    QList<QList<int> > m_towers;  // 三个柱子，每个柱子是一个盘子大小的列表
    int m_diskCount;               // 盘子数量
    int m_moveCount;               // 移动步数
    
    // 演示功能数据
    QList<QPair<int, int> > m_solution;  // 演示步骤列表
    int m_currentStep;                    // 当前演示步骤
    QTimer* m_demoTimer;                  // 演示定时器
    
    // 常量
    static const int SCENE_WIDTH = 800;
    static const int SCENE_HEIGHT = 400;
};

#endif // MAINWINDOW_SIMPLE_H 