#ifndef OWN_NAVIGATION_HPP
#define OWN_NAVIGATION_HPP

#include <iostream>
#include "navigation_planner.hpp"

namespace navigation {
/**
 * @brief 实现救援车导航类
 *
 * 继承自NavigationPlanner，实现具体的路径规划功能
 */
class own_navigation : public NavigationPlanner {
public:
    own_navigation() = default;
    ~own_navigation() = default;
    /**
     * @brief 配置地图
     * @param 二维向量存放地图
     * @return true 配置成功，false 配置失败
     */
    bool configureMap(const std::vector<std::vector<int>>& map) override;
    /**
     * @brief 显示当前地图
     */
    void displayMap() const override;
    /**
     * @brief 使用深度优先搜索规划路径
     * @param start 起始位置
     * @param goal 目标位置
     * @return true 找到路径，false 未找到路径
     */

    bool planPathBFS(const Pose2D& start, const Pose2D& goal) override;
    /**
     * @brief 检查位置是否有效
     * @param pose 要检查的位置
     * @return true 位置有效，false 位置无效
     */
    bool isValidPose(const Pose2D& pose) const override;
    /**
     * @brief 移动到受困者位置
     * @return true 移动成功，false 移动失败
     */
    bool move2Victim() const override;
    /**
     * @brief 标记受困者位置
     * @param x x坐标
     * @param y y坐标
     * @return true 标记成功，false 标记失败
     */
    bool victimMark(int x, int y) override;
};
}  // namespace navigation
#endif