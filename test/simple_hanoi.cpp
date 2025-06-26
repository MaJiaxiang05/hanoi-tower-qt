#include <iostream>
#include <vector>
#include <stack>
#include <string>
#include <thread>
#include <chrono>

using namespace std;

// 1. 游戏逻辑类 - 最核心的类
class HanoiGame {
private:
    vector<stack<int>> towers;      // 三个栈
    int diskCount;
    int moveCount;
    vector<pair<int, int>> solution;

public:
    HanoiGame(int n = 3) : diskCount(n), moveCount(0) {
        towers.resize(3);
        reset();
    }
    
    void reset() {
        // 清空所有栈
        for (auto& tower : towers) {
            while (!tower.empty()) tower.pop();
        }
        
        // 在第一根柱子上放置所有盘子（从大到小）
        for (int i = diskCount; i >= 1; i--) {
            towers[0].push(i);
        }
        moveCount = 0;
        solution.clear();
    }
    
    bool moveDisk(int from, int to) {
        if (from < 0 || from > 2 || to < 0 || to > 2 || from == to) {
            return false;
        }
        
        if (towers[from].empty()) {
            return false;
        }
        
        if (!towers[to].empty() && towers[from].top() > towers[to].top()) {
            return false;  // 大盘不能放在小盘上
        }
        
        // 执行移动
        int disk = towers[from].top();
        towers[from].pop();
        towers[to].push(disk);
        moveCount++;
        return true;
    }
    
    bool isWon() {
        return towers[2].size() == diskCount;
    }
    
    void generateSolution() {
        solution.clear();
        solve(diskCount, 0, 1, 2);
    }
    
    vector<pair<int, int>> getSolution() {
        return solution;
    }
    
    int getMoveCount() { return moveCount; }
    int getOptimalMoves() { return (1 << diskCount) - 1; }  // 2^n - 1
    
    vector<stack<int>> getTowers() { return towers; }
    
private:
    void solve(int n, int from, int aux, int to) {
        if (n == 1) {
            solution.push_back({from, to});
        } else {
            solve(n - 1, from, to, aux);
            solution.push_back({from, to});
            solve(n - 1, aux, from, to);
        }
    }
};

// 2. 显示类 - 负责所有输出
class GameDisplay {
public:
    void showTowers(const vector<stack<int>>& towers) {
        clearScreen();
        cout << "\n=== 汉诺塔游戏 ===\n\n";
        
        // 显示柱子状态
        for (int i = 0; i < 3; i++) {
            cout << "柱子 " << char('A' + i) << ": ";
            drawTower(towers[i]);
            cout << "\n";
        }
        cout << "\n";
    }
    
    void showMenu() {
        cout << "操作选项:\n";
        cout << "1. 手动移动 (输入: 1 A B - 从A移动到B)\n";
        cout << "2. 开始演示 (输入: 2)\n";
        cout << "3. 重置游戏 (输入: 3)\n";
        cout << "4. 查看帮助 (输入: 4)\n";
        cout << "5. 退出游戏 (输入: 5)\n";
        cout << "请选择: ";
    }
    
    void showMessage(const string& msg) {
        cout << ">>> " << msg << "\n";
    }
    
    void showGameInfo(int currentMoves, int optimalMoves) {
        cout << "当前步数: " << currentMoves;
        cout << " | 最优步数: " << optimalMoves << "\n\n";
    }
    
    void showHelp() {
        cout << "\n=== 游戏规则 ===\n";
        cout << "1. 目标：将所有盘子从柱子A移动到柱子C\n";
        cout << "2. 规则：\n";
        cout << "   - 一次只能移动一个盘子\n";
        cout << "   - 只能移动柱子顶部的盘子\n";
        cout << "   - 大盘子不能放在小盘子上面\n\n";
    }
    
    void clearScreen() {
        system("cls");  // Windows
        // system("clear");  // Linux/Mac
    }
    
private:
    void drawTower(stack<int> tower) {
        vector<int> disks;
        while (!tower.empty()) {
            disks.push_back(tower.top());
            tower.pop();
        }
        
        if (disks.empty()) {
            cout << "[空]";
        } else {
            cout << "[";
            for (int i = disks.size() - 1; i >= 0; i--) {
                cout << disks[i];
                if (i > 0) cout << ",";
            }
            cout << "]";
        }
    }
};

// 3. 控制器类 - 协调游戏流程
class GameController {
private:
    HanoiGame game;
    GameDisplay display;
    
public:
    GameController(int diskCount = 3) : game(diskCount) {}
    
    void run() {
        display.showHelp();
        
        while (true) {
            display.showTowers(game.getTowers());
            display.showGameInfo(game.getMoveCount(), game.getOptimalMoves());
            
            if (game.isWon()) {
                display.showMessage("恭喜！游戏完成！");
                if (game.getMoveCount() == game.getOptimalMoves()) {
                    display.showMessage("完美！您达到了最优解！");
                }
                cout << "\n";
            }
            
            display.showMenu();
            
            int choice;
            cin >> choice;
            
            switch (choice) {
                case 1:
                    handleManualMove();
                    break;
                case 2:
                    startDemo();
                    break;
                case 3:
                    game.reset();
                    display.showMessage("游戏已重置");
                    break;
                case 4:
                    display.showHelp();
                    break;
                case 5:
                    display.showMessage("谢谢游戏！再见！");
                    return;
                default:
                    display.showMessage("无效选择，请重新输入");
            }
            
            cout << "\n按Enter键继续...";
            cin.ignore();
            cin.get();
        }
    }
    
private:
    void handleManualMove() {
        char from, to;
        cout << "输入移动 (格式: A B): ";
        cin >> from >> to;
        
        int fromIndex = toupper(from) - 'A';
        int toIndex = toupper(to) - 'A';
        
        if (game.moveDisk(fromIndex, toIndex)) {
            display.showMessage("移动成功！");
        } else {
            display.showMessage("移动失败！请检查是否符合规则");
        }
    }
    
    void startDemo() {
        display.showMessage("开始演示最优解法...");
        
        game.reset();
        game.generateSolution();
        auto solution = game.getSolution();
        
        for (auto move : solution) {
            this_thread::sleep_for(chrono::milliseconds(1000));  // 延时1秒
            
            game.moveDisk(move.first, move.second);
            display.showTowers(game.getTowers());
            
            char fromChar = 'A' + move.first;
            char toChar = 'A' + move.second;
            display.showMessage("步骤 " + to_string(game.getMoveCount()) + 
                              ": 从柱子 " + fromChar + " 移动到柱子 " + toChar);
            display.showGameInfo(game.getMoveCount(), game.getOptimalMoves());
        }
        
        display.showMessage("演示完成！");
    }
};

// 主程序
int main() {
    cout << "欢迎来到汉诺塔游戏！\n";
    cout << "请输入盘子数量 (1-10): ";
    
    int diskCount;
    cin >> diskCount;
    
    if (diskCount < 1 || diskCount > 10) {
        cout << "盘子数量应在1-10之间，使用默认值3\n";
        diskCount = 3;
    }
    
    GameController controller(diskCount);
    controller.run();
    
    return 0;
} 