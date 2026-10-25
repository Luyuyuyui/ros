#include "own_navigation.hpp"
#include <iostream>
#include <vector>
#include <stack>
#include <queue>
#include <algorithm>

namespace navigation {
/**
 * @brief 配置地图
 * @param map 二维向量存放地图
 * @return true 配置成功，false 配置失败
 */
bool own_navigation::configureMap(const std::vector<std::vector<int>>& map) {
    if (map.empty()) {
        std::cout << "错误！地图不能为空！" << "\n";
        return false;
    }
    _map.width = map[0].size();
    _map.height = map.size();
    _map.map = map;
    return true;
}
/**
 * @brief 显示当前地图
 */
void own_navigation::displayMap() const {
    for (const auto& i : _map.map) {  // 遍历行
        for (int j : i) {             // 遍历列
            std::cout << j << " ";    // 输出每一个坐标
        }
        std::cout << "\n";
    }
    std::cout << "环境地图生成成功！" << "\n";
}
/**
 * @brief 标记受困者位置
 * @param x x坐标
 * @param y y坐标
 * @return true 标记成功，false 标记失败
 */
bool own_navigation::victimMark(int x, int y) {
    if (x < 0 || x >= _map.width || y < 0 || y >= _map.height) {  // 检查输入合法
        return false;
    }
    if (_map.map[y][x] == 0) {  // 如果与障碍物重合
        std::cout << "该坐标不合法，此处为障碍物！" << "\n";
        return false;
    }
    _map.map[y][x] = 3;  // 受困者标记为3
    return true;
}
/**
 * @brief 使用广度优先搜索规划路径
 * @param start 起始位置
 * @param goal 目标位置
 * @return true 找到路径，false 未找到路径
 */
bool own_navigation::planPathBFS(const Pose2D& start, const Pose2D& goal) {
    if (!isValidPose(start) || !isValidPose(goal)) {  // 检查起点和终点是否合法
        std::cout << "请重新输入正确的坐标！" << "\n";
        return false;
    }
    if (start.x == goal.x && start.y == goal.y) {  // 如果起点就是终点
        nav_path = {start};
        std::cout << "起点与终点重合" << "\n";
        return true;
    }
    nav_path.clear();  // 清空路径

    std::vector<std::vector<bool>> visited(_map.height, std::vector<bool>(_map.width, false));              // 标记访问过的地方
    std::vector<std::vector<Pose2D>> parent(_map.height, std::vector<Pose2D>(_map.width, Pose2D(-1, -1)));  // 标记父节点，便于回溯
    std::queue<Pose2D> q;                                                                                   // 创建BFS队列，用于存储待探索的节点

    q.push(start);                       // 起点入队
    int sx = static_cast<int>(start.x);  // 将起点坐标变味int类型，作为索引使用
    int sy = static_cast<int>(start.y);
    visited[sy][sx] = true;  // 标记已访问
    parent[sy][sx] = start;  // 起点的父节点设为自身（作为回溯的终止条件）

    bool found = false;

    while (!q.empty() && !found) {
        Pose2D temp = q.front();  // 获取队列前端的节点（当前探索节点）
        q.pop();                  // 弹出已获取的节点

        if (temp.x == goal.x && temp.y == goal.y) {  // 如果找到了
            found = true;
            break;
        }

        std::vector<Pose2D> walk = {
            // 坐标移动向量
            {temp.x, temp.y - 1, Heading::UP},
            {temp.x, temp.y + 1, Heading::DOWN},
            {temp.x - 1, temp.y, Heading::LEFT},
            {temp.x + 1, temp.y, Heading::RIGHT}
        };

        for (const auto& pos : walk) {  // 遍历所有可移动的地方
            int x = static_cast<int>(pos.x);
            int y = static_cast<int>(pos.y);

            if (x >= 0 && x < _map.width && y >= 0 && y < _map.height && (_map.map[y][x] == 1 || _map.map[y][x] == 3) && !visited[y][x])  // 对所有可以走的地方进行标记
            {
                visited[y][x] = true;
                parent[y][x] = temp;  // 记录父节点，用于后续路径回溯
                q.push(pos);          // 可行进的坐标入队
            }
        }
    }

    if (found) {
        std::vector<Pose2D> temp_path;  // 临时存储反向路径
        Pose2D current = goal;          // 从目标点开始反向追踪
        // 通过parent矩阵回溯整条路径
        while (!(current.x == start.x && current.y == start.y)) {
            temp_path.push_back(current);                                                // 将当前节点加入临时路径
            current = parent[static_cast<int>(current.y)][static_cast<int>(current.x)];  // 移动到父节点，继续回溯
        }
        temp_path.push_back(start);  // 将起点加入路径（循环条件会跳过起点）
                                     // 反向构建（目标->起点），需要反转得到正向路径
        std::reverse(temp_path.begin(), temp_path.end());
        nav_path = temp_path;  // 将构建好的路径赋值给输出参数
        return true;
    }

    nav_path.clear();  // 如果没有找到路径，清空导航路径，确保输出为空
    return false;
}
/**
 * @brief 检查位置是否有效
 * @param pose 要检查的位置
 * @return true 位置有效，false 位置无效
 */
bool own_navigation::isValidPose(const Pose2D& pose) const {
    int x = static_cast<int>(pose.x);
    int y = static_cast<int>(pose.y);
    // x坐标在[0, width-1]范围内+ y坐标在[0, height-1]范围内+该位置是可通行(1)或是受困者位置(3)
    return x >= 0 && x < _map.width && y >= 0 && y < _map.height && (_map.map[y][x] == 1 || _map.map[y][x] == 3);
}
/**
 * @brief 移动到受困者位置
 * @return true 移动成功，false 移动失败
 * 输出完整的移动路径
 */
bool own_navigation::move2Victim() const {
    if (nav_path.empty()) {
        return false;
    }  // 如果没有路可以走
    std::cout << "执行导航命令" << "\n";
    for (const auto& pos : nav_path) {  // 遍历已获取路径
        std::cout << "当前位置 (" << static_cast<int>(pos.x) << ", " << static_cast<int>(pos.y) << ")" << "\n";
    }
    std::cout << "导航执行完毕！" << "\n";
    return true;
}

}  // namespace navigation
