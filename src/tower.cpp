#include "tower.h"
#include <QPainter>
#include <QLinearGradient>
#include <QPen>
#include <QBrush>

Tower::Tower(int index, QGraphicsItem *parent)
    : QGraphicsItem(parent)
    , m_index(index)
    , m_highlighted(false)
{
}

QRectF Tower::boundingRect() const
{
    return QRectF(-BASE_WIDTH/2, -TOWER_HEIGHT, BASE_WIDTH, TOWER_HEIGHT + BASE_HEIGHT);
}

void Tower::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)
    
    painter->setRenderHint(QPainter::Antialiasing);
    
    // 绘制底座
    QRectF baseRect(-BASE_WIDTH/2, -BASE_HEIGHT/2, BASE_WIDTH, BASE_HEIGHT);
    QLinearGradient baseGradient(0, -BASE_HEIGHT/2, 0, BASE_HEIGHT/2);
    baseGradient.setColorAt(0, QColor(139, 69, 19).lighter(120));  // 浅棕色
    baseGradient.setColorAt(1, QColor(139, 69, 19).darker(120));   // 深棕色
    
    painter->setBrush(QBrush(baseGradient));
    painter->setPen(QPen(QColor(101, 67, 33), 2));
    painter->drawRoundedRect(baseRect, 5, 5);
    
    // 绘制柱子
    QRectF towerRect(-TOWER_WIDTH/2, -TOWER_HEIGHT, TOWER_WIDTH, TOWER_HEIGHT);
    QLinearGradient towerGradient(-TOWER_WIDTH/2, 0, TOWER_WIDTH/2, 0);
    
    QColor towerColor = m_highlighted ? QColor(255, 215, 0) : QColor(160, 82, 45);  // 金色或棕色
    towerGradient.setColorAt(0, towerColor.darker(120));
    towerGradient.setColorAt(0.5, towerColor);
    towerGradient.setColorAt(1, towerColor.darker(120));
    
    painter->setBrush(QBrush(towerGradient));
    painter->setPen(QPen(towerColor.darker(150), 2));
    painter->drawRoundedRect(towerRect, 8, 8);
    
    // 绘制柱子标签
    painter->setPen(QPen(Qt::white, 1));
    painter->setFont(QFont("Arial", 12, QFont::Bold));
    QChar label = 'A' + m_index;
    painter->drawText(baseRect, Qt::AlignCenter, QString(label));
    
    // 如果高亮，绘制边框
    if (m_highlighted) {
        painter->setPen(QPen(Qt::yellow, 3));
        painter->drawRoundedRect(baseRect.adjusted(-2, -2, 2, 2), 7, 7);
    }
}

void Tower::addDisk(Disk *disk)
{
    if (disk && canPlaceDisk(disk)) {
        m_disks.append(disk);
        disk->setParentItem(this);
        updateDiskPositions();
    }
}

Disk* Tower::removeTopDisk()
{
    if (m_disks.isEmpty()) {
        return nullptr;
    }
    
    Disk* disk = m_disks.takeLast();
    disk->setParentItem(nullptr);
    updateDiskPositions();
    return disk;
}

Disk* Tower::getTopDisk() const
{
    return m_disks.isEmpty() ? nullptr : m_disks.last();
}

bool Tower::canPlaceDisk(Disk *disk) const
{
    if (!disk) return false;
    if (m_disks.isEmpty()) return true;
    
    Disk* topDisk = getTopDisk();
    return topDisk && disk->getSize() < topDisk->getSize();
}

QPointF Tower::getDiskPosition(int index) const
{
    if (index < 0 || index >= m_disks.size()) {
        return QPointF(0, 0);
    }
    
    // 计算盘子在柱子上的位置
    qreal y = -BASE_HEIGHT/2 - (index + 1) * (20 + DISK_SPACING);  // 20是盘子高度
    return QPointF(0, y);
}

QPointF Tower::getTopPosition() const
{
    qreal y = -BASE_HEIGHT/2 - (m_disks.size() + 1) * (20 + DISK_SPACING);
    return QPointF(0, y);
}

void Tower::clear()
{
    // 删除所有盘子
    for (Disk* disk : m_disks) {
        if (disk) {
            delete disk;
        }
    }
    m_disks.clear();
}

void Tower::removeAllDisks()
{
    // 移除所有盘子但不删除它们
    for (Disk* disk : m_disks) {
        if (disk) {
            disk->setParentItem(nullptr);
        }
    }
    m_disks.clear();
}

void Tower::updateDiskPositions()
{
    for (int i = 0; i < m_disks.size(); ++i) {
        if (m_disks[i]) {
            QPointF pos = getDiskPosition(i);
            m_disks[i]->setPosition(pos);
        }
    }
} 