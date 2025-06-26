#ifndef TOWER_H
#define TOWER_H

#include <QGraphicsItem>
#include <QList>
#include "disk.h"

class Tower : public QGraphicsItem
{
public:
    explicit Tower(int index, QGraphicsItem *parent = nullptr);
    
    // QGraphicsItem接口
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, 
               QWidget *widget = nullptr) override;
    
    // 盘子操作
    void addDisk(Disk *disk);
    Disk* removeTopDisk();
    Disk* getTopDisk() const;
    bool canPlaceDisk(Disk *disk) const;
    bool isEmpty() const { return m_disks.isEmpty(); }
    int getDiskCount() const { return m_disks.size(); }
    QList<Disk*> getDisks() const { return m_disks; }
    
    // 位置计算
    QPointF getDiskPosition(int index) const;
    QPointF getTopPosition() const;
    int getTowerIndex() const { return m_index; }
    
    // 清理
    void clear();
    void removeAllDisks();
    
    // 视觉效果
    void setHighlight(bool highlight) { m_highlighted = highlight; update(); }
    bool isHighlighted() const { return m_highlighted; }
    
private:
    void updateDiskPositions();
    
private:
    QList<Disk*> m_disks;    // 柱子上的盘子列表
    int m_index;             // 柱子索引 (0, 1, 2)
    bool m_highlighted;      // 是否高亮显示
    
    static const int TOWER_WIDTH = 20;    // 柱子宽度
    static const int TOWER_HEIGHT = 300;  // 柱子高度
    static const int BASE_WIDTH = 150;    // 底座宽度
    static const int BASE_HEIGHT = 30;    // 底座高度
    static const int DISK_SPACING = 2;    // 盘子间距
};

#endif // TOWER_H 