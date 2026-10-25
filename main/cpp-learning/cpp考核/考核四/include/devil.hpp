/**
 * @file devil.hpp
 * @brief Devil类的定义
 */
#pragma once
#ifndef DEVIL_HPP
#define DEVIL_HPP
#include "robot.hpp"
#include <memory>
#include <string>
#include <vector>
#include <map>

class Devil {
public:
    /**
     * @brief 构造函数，接收环境信息字符串
     * @param env_str 环境信息字符串
     */
    explicit Devil(const std::string& env_str);
    /**
     * @brief 析构函数
     */
    ~Devil() = default;

    // 不允许复制
    Devil(const Devil&) = delete;
    Devil& operator=(const Devil&) = delete;
    // 允许移动
    Devil(Devil&& rhs) noexcept;
    Devil& operator=(Devil&& rhs) noexcept;

    /**
     * @brief 读取任务信息
     * @param task_str 任务信息字符串
     *
     * @details 解析任务字符串，存储任务编号和对应的任务信息
     */
    void readTask(const std::string& task_str);
    /**
     * @brief 按照提供任务的顺序完成所有任务
     */
    void Complete();
    /**
     * @brief 完成指定编号的任务
     * @param id 任务id
     */
    void Complete(std::size_t id);

private:
    /**
     * @brief 任务信息，存储单个任务的详细信息
     */
    struct TaskInfo {
        std::string type;                                         ///< 任务类型识别
        std::vector<std::pair<std::string, std::string>> object;  ///< 任务操作对象列表，属性+属性值"item", "bottle"
        std::map<std::string, std::string> feature;               ///< 任务特征映射表。task 1
    };
    /**
     * @brief 物品信息，存储单个物品的详细信息
     */
    struct ItemInfo {
        size_t id;            ///< 物品对应任务中的变量名XY
        std::string name;     ///< 物品名称
        std::string type;     ///< 物品类型container object
        std::string color;    ///< 物品颜色
        size_t pos;           ///< 物品位置坐标
        size_t container_id;  ///< 所在容器的ID
        bool is_open;         ///< 容器开启状态
    };
    /**
     * @brief 解析环境信息
     * @param env_str 环境描述字符串，包含所有物品的初始状态信息
     */
    void parseEnvironment(const std::string& env_str);
    /**
     * @brief 解析单行环境信息
     *  @param line 单行环境描述字符串
     */
    void parseEnvironmentLine(const std::string& line);
    /**
     * @brief 解析任务信息
     * @param task_str 任务描述字符串，包含所有任务的定义
     */
    std::map<size_t, TaskInfo> parseTask(const std::string& task_str);
    /**
     * @brief 解析单行任务信息
     * @param line 单行任务描述字符串
     * @param task 映射对应信息，用于存储解析结果
     */
    void parseTaskLine(const std::string& line, std::map<std::size_t, TaskInfo>& task);
    /**
     * @brief 执行任务并且更新任务状态
     * @param task_id 任务ID
     * @param task_info 任务信息
     * @param items 物品信息映射表
     * @return bool 任务执行是否成功
     */
    bool executeTask(size_t task_id, const TaskInfo& task_info, const std::map<size_t, ItemInfo>& items);
    /**
     * @brief 检查任务是否已完成
     * @param task_info 要检查的任务信息
     * @return True 已完成
     */
    bool isTaskCompleted(const TaskInfo& task_info);
    /**
     * @brief 根据条件获取匹配的物品信息
     * @param feature 特征条件映射表
     * @return const ItemInfo* 指向匹配物品信息的指针，未找到时返回nullptr
     */
    const ItemInfo* getItemInfo(const std::map<std::string, std::string>& feature) const;
    /**
     * @brief 更新物品位置
     * @param item_id 物品ID
     * @param new_position 新的位置坐标
     */
    void updateItemPosition(size_t item_id, size_t new_position);
    /**
     * @brief 更新物品所在容器
     * @param item_id 物品ID
     * @param container_id 容器ID
     * @details 当当物品被放入或取出容器时，需要更新物品的位置以及归属关系
     */
    void updateItemContainer(size_t item_id, size_t container_id);
    /**
     * @brief 更新容器打开状态
     * @param container_id 容器ID
     * @param is_open 容器是否打开
     * @details 更新容器的打开/关闭状态，决定容器内物品的可访问性
     */
    void updateContainerOpenState(size_t container_id, bool is_open);
    /**
     * @brief 更新容器内物品的位置
     * @details 当容器移动时，自动更新容器内所有物品的位置信息
     */
    void updateContainerItemPositions();
    std::unique_ptr<Robot> robot_;     ///< 机器人实例指针，负责实际物理操作
    std::map<size_t, ItemInfo> item_;  ///< 物品编号到物品信息的映射
    std::map<size_t, TaskInfo> task_;  ///< 任务编号到任务信息的映射
    std::vector<size_t> task_order;    ///< 任务执行顺序（按任务编号排序）

    std::map<size_t, bool> task_completed;  ///< 任务完成状态映射
};
#endif