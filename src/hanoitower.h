#ifndef HANOITOWER_H
#define HANOITOWER_H

#include <QObject>
#include <QList>
#include <QPair>
#include "tower.h"
#include "disk.h"

class HanoiTower : public QObject
{
    Q_OBJECT

public:
    explicit HanoiTower(int diskCount = 3, QObject *parent = nullptr);
    ~HanoiTower();
    
    // 游戏控制
    void initialize();
    void reset();
    bool moveDisk(int fromTower, int toTower);
    bool isGameWon() const;
    
    // 获取游戏状态
    int getDiskCount() const { return m_diskCount; }
    int getMoveCount() const { return m_moveCount; }
    int getOptimalMoves() const { return (1 << m_diskCount) - 1; }  // 2^n - 1
    QList<Tower*> getTowers() const { return m_towers; }
    Tower* getTower(int index) const;
    
    // 自动解决
    void generateSolution();
    QList<QPair<int, int>> getSolution() const { return m_solution; }
    void clearSolution() { m_solution.clear(); }
    
    // 设置盘子数量
    void setDiskCount(int count);
    
    // 验证移动
    bool isValidMove(int fromTower, int toTower) const;
    QString getLastErrorMessage() const { return m_lastError; }
    
signals:
    void diskMoved(int from, int to, Disk* disk);
    void gameWon();
    void moveCountChanged(int count);
    void gameStateChanged();
    void invalidMoveAttempted(const QString &message);
    
private:
    void solveRecursive(int n, int from, int aux, int to);
    void createDisks();
    void clearGame();
    
private:
    QList<Tower*> m_towers;           // 三根柱子
    QList<Disk*> m_disks;            // 所有盘子
    int m_diskCount;                  // 盘子数量
    int m_moveCount;                  // 当前移动步数
    QList<QPair<int, int>> m_solution; // 最优解路径
    QString m_lastError;              // 最后的错误信息
    
    static const int TOWER_COUNT = 3; // 柱子数量
};

#endif // HANOITOWER_H 