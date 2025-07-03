#include "hanoigame.h"


HanoiGame::HanoiGame(QObject *parent)
    : QObject(parent)
    , m_diskCount(3)
    , m_moveCount(0)
{
    initializeGame(3);
}

void HanoiGame::initializeGame(int diskCount)
{
    m_diskCount = diskCount;
    m_moveCount = 0;
    
    m_towers.clear();
    int i;
    for (i = 0; i < 3; ++i) {
        m_towers.append(QList<int>());
    }
    
    // 将所有盘子放在第一个柱子上，从大到小
    for (i = m_diskCount; i >= 1; --i) {
        m_towers[0].append(i);
    }
}

void HanoiGame::resetGame()
{
    initializeGame(m_diskCount);
}

bool HanoiGame::moveDisk(int from, int to)
{
    if (from < 0 || from >= 3 || to < 0 || to >= 3 || from == to) {
        emit invalidMove("无效的柱子索引");
        return false;
    }
    
    if (m_towers[from].isEmpty()) {
        emit invalidMove(QString("柱子 %1 上没有盘子").arg(char('A' + from)));
        return false;
    }
    
    int disk = m_towers[from].last();
    
    if (!m_towers[to].isEmpty() && m_towers[to].last() < disk) {
        emit invalidMove("不能将大盘子放在小盘子上");
        return false;
    }
    
    // 执行移动
    m_towers[from].removeLast();
    m_towers[to].append(disk);
    m_moveCount++;
    
    emit moveExecuted(from, to, disk);
    
    // 检查是否获胜
    if (isGameWon()) {
        emit gameWon();
    }
    
    return true;
}

bool HanoiGame::isGameWon() const
{
    return m_towers[2].size() == m_diskCount;
}

int HanoiGame::getMoveCount() const
{
    return m_moveCount;
}

int HanoiGame::getDiskCount() const
{
    return m_diskCount;
}

int HanoiGame::getOptimalMoves() const
{
    // 计算最优步数为(2^n - 1)
    int optimal = 1;
    int n;
    for (n = 0; n < m_diskCount; ++n) {
        optimal *= 2;
    }
    return optimal - 1;
}

QList<QList<int> > HanoiGame::getTowers() const
{
    return m_towers;
}

QList<QPair<int, int> > HanoiGame::generateSolution()
{
    QList<QPair<int, int> > solution;
    solveHanoi(m_diskCount, 0, 2, 1, solution);
    return solution;
}

void HanoiGame::solveHanoi(int n, int from, int to, int aux, QList<QPair<int, int> > &solution)
{
    if (n == 1) {
        solution.append(QPair<int, int>(from, to));
    } else {
        solveHanoi(n - 1, from, aux, to, solution);
        solution.append(QPair<int, int>(from, to));
        solveHanoi(n - 1, aux, to, from, solution);
    }
} 
 