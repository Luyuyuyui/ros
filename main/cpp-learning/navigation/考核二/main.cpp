#include <vector>
#include <iostream>
#include "own_navigation.hpp"
using namespace navigation;
/**
 * @brief 从键盘输入目标位置
 * @param mx 地图最大x坐标
 * @param my 地图最大y坐标
 * @return 输入的目标位置
 */
Pose2D inputGoal(int mx, int my) {
    int x, y;
    std::cout << "请输入目标位置" << "\n";
    while (1) {
        std::cin >> x >> y;                          // 输入目标位置
        std::cin.ignore(10000, '\n');                // 清理缓冲区，保证下一次循环正常
        if (x >= 0 && x < mx && y >= 0 && y < my) {  // 检查合法
            break;
        } else {
            std::cout << "请重新输入！" << "\n";
        }
    }
    return Pose2D(x, y);
}
/**
 * @brief 主函数，实现救援车导航功能
 * @return 程序结束
 */
int main() {
    own_navigation user;  // 实例化对象
    std::vector<std::vector<int>> map = {{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, {1, 1, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1}, {1, 1, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1},
                                         {1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1}, {1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1}, {1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                                         {1, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1}, {1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1}, {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                                         {1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, {1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1}, {1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1},
                                         {1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1}, {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                                         {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}};
    if (!user.configureMap(map)) {
        std::cout << "地图配置失败！" << "\n";
        return -1;
    } else {
        std::cout << "地图配置成功！" << "\n";
    }
    auto map_info = user.getMap();  // 获取地图信息
    std::cout << "宽=" << map_info.width << ", 高=" << map_info.height << "\n";

    user.displayMap();  // 显示地图

    Pose2D start(0, 0);
    Pose2D goal = inputGoal(map_info.width, map_info.height);  // 设定最终坐标

    if (user.victimMark(goal.x, goal.y)) {
        std::cout << "被困者位置标记成功 (" << goal.x << "," << goal.y << ")"
                  << "\n";
        user.displayMap();  // 标记成功后展示更新的地图
        std::cout << "被困者位置设置成功！" << "\n";
    }

    bool path_found = false;
    // path_found = user.planPathDFS(start, goal);
    path_found = user.planPathBFS(start, goal);  // 找路

    if (path_found) {
        std::cout << "规划成功！" << "\n";
        auto path = user.getNavPath();  // 找到了
        std::cout << "路径: ";
        user.move2Victim();
    } else {
        std::cout << "路径规划失败！无法到达目标位置。" << std::endl;
    }

    return 0;
}