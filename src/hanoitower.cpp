#include "hanoitower.h"
#include <QDebug>

HanoiTower::HanoiTower(int diskCount, QObject *parent)
    : QObject(parent)
    , m_diskCount(diskCount)
    , m_moveCount(0)
{
    // 创建三根柱子
    for (int i = 0; i < TOWER_COUNT; ++i) {
        m_towers.append(new Tower(i));
    }
    
    initialize();
}

HanoiTower::~HanoiTower()
{
    clearGame();
    
    // 删除柱子
    for (Tower* tower : m_towers) {
        delete tower;
    }
    m_towers.clear();
}

void HanoiTower::initialize()
{
    clearGame();
    createDisks();
    m_moveCount = 0;
    
    // 将所有盘子放在第一根柱子上（从大到小）
    for (int i = m_diskCount; i >= 1; --i) {
        Disk* disk = new Disk(i);
        m_disks.append(disk);
        m_towers[0]->addDisk(disk);
    }
    
    emit gameStateChanged();
    emit moveCountChanged(m_moveCount);
}

void HanoiTower::reset()
{
    initialize();
}

bool HanoiTower::moveDisk(int fromTower, int toTower)
{
    // 验证输入
    if (fromTower < 0 || fromTower >= TOWER_COUNT || 
        toTower < 0 || toTower >= TOWER_COUNT) {
        m_lastError = "无效的柱子索引";
        emit invalidMoveAttempted(m_lastError);
        return false;
    }
    
    if (fromTower == toTower) {
        m_lastError = "源柱子和目标柱子不能相同";
        emit invalidMoveAttempted(m_lastError);
        return false;
    }
    
    Tower* sourceTower = m_towers[fromTower];
    Tower* targetTower = m_towers[toTower];
    
    // 检查源柱子是否有盘子
    if (sourceTower->isEmpty()) {
        m_lastError = QString("柱子 %1 上没有盘子").arg(QChar('A' + fromTower));
        emit invalidMoveAttempted(m_lastError);
        return false;
    }
    
    Disk* disk = sourceTower->getTopDisk();
    
    // 检查是否可以放置到目标柱子
    if (!targetTower->canPlaceDisk(disk)) {
        m_lastError = QString("不能将大盘子放在小盘子上面");
        emit invalidMoveAttempted(m_lastError);
        return false;
    }
    
    // 执行移动
    sourceTower->removeTopDisk();
    targetTower->addDisk(disk);
    
    m_moveCount++;
    
    emit diskMoved(fromTower, toTower, disk);
    emit moveCountChanged(m_moveCount);
    emit gameStateChanged();
    
    // 检查是否获胜
    if (isGameWon()) {
        emit gameWon();
    }
    
    return true;
}

bool HanoiTower::isGameWon() const
{
    // 检查最后一根柱子是否有所有盘子
    Tower* lastTower = m_towers[TOWER_COUNT - 1];
    return lastTower->getDiskCount() == m_diskCount;
}

Tower* HanoiTower::getTower(int index) const
{
    if (index >= 0 && index < m_towers.size()) {
        return m_towers[index];
    }
    return nullptr;
}

void HanoiTower::generateSolution()
{
    m_solution.clear();
    if (m_diskCount > 0) {
        solveRecursive(m_diskCount, 0, 1, 2);  // 从A柱移动到C柱，B柱作为辅助
    }
}

void HanoiTower::setDiskCount(int count)
{
    if (count >= 1 && count <= 10) {  // 限制盘子数量在合理范围内
        m_diskCount = count;
        initialize();
    }
}

bool HanoiTower::isValidMove(int fromTower, int toTower) const
{
    if (fromTower < 0 || fromTower >= TOWER_COUNT || 
        toTower < 0 || toTower >= TOWER_COUNT) {
        return false;
    }
    
    if (fromTower == toTower) {
        return false;
    }
    
    Tower* sourceTower = m_towers[fromTower];
    Tower* targetTower = m_towers[toTower];
    
    if (sourceTower->isEmpty()) {
        return false;
    }
    
    Disk* disk = sourceTower->getTopDisk();
    return targetTower->canPlaceDisk(disk);
}

void HanoiTower::solveRecursive(int n, int from, int aux, int to)
{
    if (n == 1) {
        // 基本情况：直接移动一个盘子
        m_solution.append(qMakePair(from, to));
    } else {
        // 递归情况：
        // 1. 将前n-1个盘子从from移动到aux（使用to作为辅助）
        solveRecursive(n - 1, from, to, aux);
        
        // 2. 将最大的盘子从from移动到to
        m_solution.append(qMakePair(from, to));
        
        // 3. 将前n-1个盘子从aux移动到to（使用from作为辅助）
        solveRecursive(n - 1, aux, from, to);
    }
}

void HanoiTower::createDisks()
{
    // 清除现有盘子
    for (Disk* disk : m_disks) {
        delete disk;
    }
    m_disks.clear();
}

void HanoiTower::clearGame()
{
    // 清除所有柱子上的盘子
    for (Tower* tower : m_towers) {
        tower->clear();
    }
    
    // 清除盘子列表
    for (Disk* disk : m_disks) {
        delete disk;
    }
    m_disks.clear();
    
    m_solution.clear();
    m_lastError.clear();
} 