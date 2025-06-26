#ifndef DISK_H
#define DISK_H

#include <QGraphicsItem>
#include <QColor>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QWidget>

class Disk : public QGraphicsItem
{
public:
    explicit Disk(int size, QGraphicsItem *parent = nullptr);
    
    // QGraphicsItem接口
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, 
               QWidget *widget = nullptr) override;
    
    // 自定义方法
    int getSize() const { return m_size; }
    void setColor(const QColor &color) { m_color = color; }
    QColor getColor() const { return m_color; }
    
    // 动画支持
    void setPosition(const QPointF &pos);
    QPointF getTargetPosition() const { return m_targetPosition; }
    
    // 鼠标交互
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    
private:
    int m_size;              // 盘子大小 (1是最小的)
    QColor m_color;          // 盘子颜色
    QPointF m_targetPosition; // 目标位置
    bool m_dragging;         // 是否正在拖拽
    QPointF m_dragStart;     // 拖拽开始位置
    
    static const int DISK_HEIGHT = 20;  // 盘子高度
    static const int SIZE_MULTIPLIER = 15; // 大小倍数
};

#endif // DISK_H 