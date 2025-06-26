#include "gamedisplay.h"

GameDisplay::GameDisplay(QObject *parent)
    : QObject(parent)
    , m_view(NULL)
    , m_scene(NULL)
{
    m_scene = new QGraphicsScene(this);
    m_scene->setSceneRect(0, 0, SCENE_WIDTH, SCENE_HEIGHT);
}

GameDisplay::~GameDisplay()
{
}

void GameDisplay::setGraphicsView(QGraphicsView* view)
{
    m_view = view;
    if (m_view) {
        m_view->setScene(m_scene);
    }
}

void GameDisplay::drawGame(const QList<QList<int> > &towers, int selectedTower)
{
    clearDisplay();
    
    drawBase();
    
    // 绘制三个柱子
    int i;
    for (i = 0; i < 3; ++i) {
        drawTower(150 + i * 250, 150);
    }
    
    // 绘制盘子
    int towerIndex, diskIndex;
    for (towerIndex = 0; towerIndex < 3; ++towerIndex) {
        for (diskIndex = 0; diskIndex < towers[towerIndex].size(); ++diskIndex) {
            int diskSize = towers[towerIndex][diskIndex];
            drawDisk(diskSize, towerIndex, diskIndex);
        }
    }
    
    drawLabels();
    
    // 绘制选中状态
    if (selectedTower >= 0 && selectedTower < 3) {
        drawSelection(selectedTower);
    }
}

void GameDisplay::clearDisplay()
{
    m_scene->clear();
}

QGraphicsScene* GameDisplay::getScene() const
{
    return m_scene;
}

void GameDisplay::drawTower(int x, int y)
{
    // 绘制柱子（垂直的矩形）
    QColor brownColor(139, 69, 19);  // 棕色RGB值
    m_scene->addRect(x - TOWER_WIDTH/2, y, TOWER_WIDTH, TOWER_HEIGHT, 
                     QPen(Qt::black, 2), 
                     QBrush(brownColor));
}

void GameDisplay::drawDisk(int size, int towerIndex, int position)
{
    int baseWidth = 30;
    int width = baseWidth + size * 20;  // 盘子宽度根据大小变化
    
    int x = 150 + towerIndex * 250 - width / 2;  // 居中对齐
    int y = 330 - position * DISK_HEIGHT;        // 从底部向上堆叠
    
    QColor diskColor = getDiskColor(size);
    
    m_scene->addRect(x, y, width, DISK_HEIGHT, 
                     QPen(Qt::black, 1), 
                     QBrush(diskColor));
}

void GameDisplay::drawLabels()
{
    // 绘制柱子标签
    for (int i = 0; i < 3; i++) {
        QGraphicsTextItem* label = m_scene->addText(QString("柱子 %1").arg(char('A' + i)));
        label->setPos(120 + i * 250, 380);
        label->setFont(QFont("Arial", 12, QFont::Bold));//用简洁的无衬线字体Arial
    }
}

void GameDisplay::drawBase()
{
    // 绘制底座
    m_scene->addRect(50, 350, BASE_WIDTH, BASE_HEIGHT, 
                     QPen(Qt::black, 2), 
                     QBrush(Qt::darkGray));
}

void GameDisplay::drawSelection(int selectedTower)
{
    // 绘制红色虚线选中框
    m_scene->addRect(100 + selectedTower * 250, 140, 
                     100, 240, 
                     QPen(Qt::red, 3, Qt::DashLine), 
                     QBrush(Qt::transparent));
}

QColor GameDisplay::getDiskColor(int size)
{  
    QColor diskColor;

    switch ((size - 1) % 10) {
        case 0: diskColor = Qt::red; break;
        case 1: diskColor = Qt::blue; break;
        case 2: diskColor = Qt::green; break;
        case 3: diskColor = Qt::yellow; break;
        case 4: diskColor = Qt::magenta; break;
        case 5: diskColor = Qt::cyan; break;
        case 6: diskColor = Qt::darkRed; break;
        case 7: diskColor = Qt::darkBlue; break;
        case 8: diskColor = Qt::darkGreen; break;
        case 9: diskColor = QColor(255, 165, 0); break; // 橙色RGB值
        default: diskColor = Qt::lightGray; break;
    }

    return diskColor;
}

 
