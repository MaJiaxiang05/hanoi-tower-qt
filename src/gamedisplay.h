#ifndef GAMEDISPLAY_H
#define GAMEDISPLAY_H

#include <QObject>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
#include <QList>
#include <QColor>
#include <QPen>
#include <QBrush>
#include <QFont>

class GameDisplay : public QObject
{
    Q_OBJECT

public:
    explicit GameDisplay(QObject *parent = nullptr);
    ~GameDisplay();
    
    // 设置图形视图
    void setGraphicsView(QGraphicsView* view);
    
    // 绘制游戏界面
    void drawGame(const QList<QList<int> > &towers, int selectedTower = -1);
    // 清空界面
    void clearDisplay();
    
    // 场景管理
    QGraphicsScene* getScene() const;

private:
    // 画各种图形，标签，选中框
    void drawTower(int x, int y);
    void drawDisk(int size, int towerIndex, int position);
    void drawLabels();
    void drawBase();
    void drawSelection(int selectedTower);
    
    // 根据圆盘尺寸得到颜色
    QColor getDiskColor(int size);

private:
    QGraphicsView* m_view;
    QGraphicsScene* m_scene;
    
    // 静态常量
    static const int SCENE_WIDTH = 800;
    static const int SCENE_HEIGHT = 400;
    static const int TOWER_HEIGHT = 200;
    static const int TOWER_WIDTH = 10;
    static const int DISK_HEIGHT = 20;
    static const int BASE_WIDTH = 700;
    static const int BASE_HEIGHT = 20;
};

#endif
