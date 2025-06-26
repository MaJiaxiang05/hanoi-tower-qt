#include <QApplication>
#include <QDebug>
#include "hanoigame.h"
#include "gamedisplay.h"
#include "gamecontroller.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    // 测试所有类
    qDebug() << "=== 测试汉诺塔多文件架构 ===";
    
    // 1. 测试HanoiGame类
    HanoiGame game;
    game.initializeGame(3);
    qDebug() << "✓ HanoiGame初始化完成";
    qDebug() << "  盘子数量:" << game.getDiskCount();
    qDebug() << "  移动步数:" << game.getMoveCount();
    qDebug() << "  最优步数:" << game.getOptimalMoves();
    
    // 2. 测试GameDisplay类
    GameDisplay display;
    qDebug() << "✓ GameDisplay创建完成";
    qDebug() << "  场景大小:" << display.getScene()->sceneRect();
    
    // 3. 测试GameController类
    GameController controller;
    controller.setGame(&game);
    controller.setDisplay(&display);
    qDebug() << "✓ GameController设置完成";
    qDebug() << "  演示状态:" << controller.isDemonstrating();
    qDebug() << "  选中柱子:" << controller.getSelectedTower();
    
    // 4. 测试游戏逻辑
    if (game.moveDisk(0, 1)) {
        qDebug() << "✓ 移动测试成功！当前步数:" << game.getMoveCount();
    }
    
    qDebug() << "=== 所有测试完成！===";
    
    return 0;  // 不需要app.exec()，只是测试
} 