#include "disk.h"
#include <QGraphicsSceneMouseEvent>
#include <QLinearGradient>
#include <QPen>
#include <QBrush>

Disk::Disk(int size, QGraphicsItem *parent)
    : QGraphicsItem(parent)
    , m_size(size)
    , m_dragging(false)
{
    // 根据大小设置颜色
    QColor baseColor;
    switch (size % 7) {
    case 1: baseColor = QColor(255, 99, 132); break;   // 红色
    case 2: baseColor = QColor(54, 162, 235); break;   // 蓝色
    case 3: baseColor = QColor(255, 205, 86); break;   // 黄色
    case 4: baseColor = QColor(75, 192, 192); break;   // 青色
    case 5: baseColor = QColor(153, 102, 255); break;  // 紫色
    case 6: baseColor = QColor(255, 159, 64); break;   // 橙色
    case 0: baseColor = QColor(201, 203, 207); break;  // 灰色
    }
    m_color = baseColor;
    
    // 启用鼠标事件
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setAcceptHoverEvents(true);
}

QRectF Disk::boundingRect() const
{
    int width = m_size * SIZE_MULTIPLIER;
    return QRectF(-width/2, -DISK_HEIGHT/2, width, DISK_HEIGHT);
}

void Disk::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)
    
    int width = m_size * SIZE_MULTIPLIER;
    QRectF rect(-width/2, -DISK_HEIGHT/2, width, DISK_HEIGHT);
    
    // 创建渐变效果
    QLinearGradient gradient(0, -DISK_HEIGHT/2, 0, DISK_HEIGHT/2);
    gradient.setColorAt(0, m_color.lighter(120));
    gradient.setColorAt(0.5, m_color);
    gradient.setColorAt(1, m_color.darker(120));
    
    // 绘制盘子
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setBrush(QBrush(gradient));
    painter->setPen(QPen(m_color.darker(150), 2));
    painter->drawRoundedRect(rect, 5, 5);
    
    // 如果被选中，添加高亮边框
    if (isSelected()) {
        painter->setPen(QPen(Qt::yellow, 3));
        painter->drawRoundedRect(rect, 5, 5);
    }
    
    // 绘制大小标识
    painter->setPen(QPen(Qt::white, 1));
    painter->setFont(QFont("Arial", 10, QFont::Bold));
    painter->drawText(rect, Qt::AlignCenter, QString::number(m_size));
}

void Disk::setPosition(const QPointF &pos)
{
    m_targetPosition = pos;
    setPos(pos);
}

void Disk::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_dragging = true;
        m_dragStart = event->pos();
        setSelected(true);
    }
    QGraphicsItem::mousePressEvent(event);
}

void Disk::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (m_dragging) {
        // 在拖拽过程中更新位置
        setPos(mapToParent(event->pos() - m_dragStart));
    }
    QGraphicsItem::mouseMoveEvent(event);
}

void Disk::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_dragging = false;
        setSelected(false);
    }
    QGraphicsItem::mouseReleaseEvent(event);
} 