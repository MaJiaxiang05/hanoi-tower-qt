#include <QApplication>
#include <QDebug>
#include "hanoigame.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    qDebug() << "开始测试HanoiGame类...";
    
    // 测试HanoiGame类
    HanoiGame* game = new HanoiGame();
    
    qDebug() << "✓ HanoiGame对象创建成功";
    qDebug() << "  盘子数量:" << game->getDiskCount();
    qDebug() << "  移动步数:" << game->getMoveCount();
    qDebug() << "  最优步数:" << game->getOptimalMoves();
    
    // 测试移动
    if (game->moveDisk(0, 1)) {
        qDebug() << "✓ 移动测试成功！";
        qDebug() << "  当前步数:" << game->getMoveCount();
    } else {
        qDebug() << "✗ 移动测试失败！";
    }
    
    delete game;
    qDebug() << "测试完成！";
    
    return 0;
} 