#ifndef HANOIGAME_H
#define HANOIGAME_H

#include <QObject>
#include <QList>
#include <QPair>
#include <QDebug>

class HanoiGame : public QObject
{
    Q_OBJECT

public:
    explicit HanoiGame(QObject *parent = nullptr);
    
    // 游戏基本操作
    void initializeGame(int diskCount);
    void resetGame();
    bool moveDisk(int from, int to);
    bool isGameWon() const;
    
    // 获取游戏状态
    int getMoveCount() const;
    int getDiskCount() const;
    int getOptimalMoves() const;
    QList<QList<int> > getTowers() const;
    
    // 演示功能
    QList<QPair<int, int> > generateSolution();
    
signals:
    void gameWon();
    void moveExecuted(int from, int to, int diskSize);
    void invalidMove(const QString &reason);

private:
    void solveHanoi(int n, int from, int to, int aux, QList<QPair<int, int> > &solution);
    
private:
    QList<QList<int> > m_towers;     // 三个柱子的盘子
    int m_diskCount;                  // 盘子数量
    int m_moveCount;                  // 移动步数
};

#endif // HANOIGAME_H 
