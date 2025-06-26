#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    // 设置应用程序信息
    app.setApplicationName("汉诺塔游戏");
    app.setApplicationVersion("1.0");
    app.setOrganizationName("HanoiTower");
    
    MainWindow window;
    window.show();
    
    return app.exec();
} 