// task_executor.cpp
#include "devil.hpp"
#include <iostream>
/**
 * @brief 执行具体任务
 * @param task_id 任务ID
 * @param task_info 任务信息
 * @param items 物品信息映射表
 * @return bool 任务执行是否成功
 * @details 根据任务类型执行不同的操作序列
 */
bool Devil::executeTask(size_t task_id, const TaskInfo& task_info, const std::map<size_t, ItemInfo>& items) {
    bool success = false;                // 标记任务是否成功
    static size_t current_position = 0;  // 静态变量，记录机器人当前位置
    //  Lambda函数：先解析目标名称及类型，再遍历所有物品匹配目标
    auto getItemByFeature = [&](const std::string& key, const std::string& expected_type = "") -> const ItemInfo* {
        // feature:{"X": "bottle", "Y": "desk"} 中查找 key="Y"
        auto feature_it = task_info.feature.find(key);
        if (feature_it == task_info.feature.end()) return nullptr;  // 没找到
        const std::string& target_name = feature_it->second;        // 存储目标名称
        // 遍历所有物品，查找匹配的物品
        for (const auto& pair : items) {
            if (pair.second.name == target_name && (expected_type.empty() || pair.second.type == expected_type)) {
                return &pair.second;  // 物品名称=目标名称+物品类型=期望类型
            }
        }
        return nullptr;
    };
    // Lambda函数：机器人移动到指定位置
    auto moveTo = [&](size_t pos) -> bool {
        if (current_position == pos) return true;  // 已经在目标位置
        if (!robot_->move(pos)) return false;      // 移动失败
        current_position = pos;                    // 更新当前位置
        return true;
    };
    // Lambda函数：从容器中取出物品
    auto takeOutFromContainer = [&](const ItemInfo* item) -> bool {
        // 查找容器信息
        if (!item || item->container_id == 0) return true;  // 物品不存在或不在容器内
        auto container_it = items.find(item->container_id);
        if (container_it == items.end()) return false;
        const ItemInfo* container = &container_it->second;
        // 机器人移动到容器位置
        if (!moveTo(container->pos)) return false;
        // 如果容器关闭，先打开容器
        if (!container->is_open) {
            if (!robot_->open(container->id)) return false;  // 如果机器人打开容器失败
            updateContainerOpenState(container->id, true);   // 机器人打开容器
        }
        if (!robot_->takeOut(item->id, container->id)) return false;  // 如果机器人拿出物品失败
        updateItemContainer(item->id, 0);                             // 取出物品，物品所在容器设置为0
        return true;
    };
    // Lambda函数：处理容器开关操作
    auto handleContainerOperation = [&](const ItemInfo* container, bool should_open) -> bool {
        if (container->is_open == should_open) return true;  // 已是目标状态
        // 移动到容器位置并执行开关操作
        if (!moveTo(container->pos)) return false;                                               // 不能移动到容器位置
        bool result = should_open ? robot_->open(container->id) : robot_->close(container->id);  // 开关操作
        if (result) {                                                                            // 如果执行完成，更新开关状态
            updateContainerOpenState(container->id, should_open);
        }
        return result;
    };

    const std::string& type = task_info.type;
    const ItemInfo* target_item = getItemInfo(task_info.feature);
    // 接下来开始处理具体任务
    if (type == "giveme") {
        if (!target_item || !takeOutFromContainer(target_item) || !moveTo(target_item->pos) || !robot_->pickUp(target_item->id) || !moveTo(0) || !robot_->putDown(target_item->id)) return false;
        // 物品存在+如果在容器内，取出物品（移动到容器位置-打开容器-取出物品）||移动到物品位置+拾取物品+移动到0（人类位置）+放下物品
        updateItemPosition(target_item->id, 0);  // 更新物品位置
        success = true;
    } else if (type == "puton") {
        const ItemInfo* big_item = getItemByFeature("Y", "big");  // 查找大物品
        if (!target_item || !big_item || !takeOutFromContainer(target_item) || !moveTo(target_item->pos) || !robot_->pickUp(target_item->id) || !moveTo(big_item->pos) ||
            !robot_->putDown(target_item->id))
            // 物品存在+如果在容器内，取出小物品（移动到容器位置-打开容器-取出物品）||移动到小物品位置+拾取+移动到大物品位置→放下
            return false;
        updateItemPosition(target_item->id, big_item->pos);  // 更新小物品位置到大物品位置
        success = true;
    } else if (type == "putin") {
        const ItemInfo* container = getItemByFeature("Y", "container");  // 查找容器
        if (!target_item || !container || !takeOutFromContainer(target_item) || !moveTo(target_item->pos) || !robot_->pickUp(target_item->id) || !moveTo(container->pos)) return false;
        // 物品容器存在+如果在容器内，取出物品（移动到容器位置-打开容器-取出物品）||移动到物品位置+拾取→移动到容器位置→放入容器
        if (!handleContainerOperation(container, true) || !robot_->putIn(target_item->id, container->id)) return false;
        // 确保容器打开并将物品放入
        updateItemPosition(target_item->id, container->pos);  // 更新物品位置
        updateItemContainer(target_item->id, container->id);  // 更新物品和容器的相对位置
        success = true;
    } else if (type == "takeout") {
        const ItemInfo* container = getItemByFeature("Y", "container");  // 查找容器
        if (!target_item || !container) return false;
        if (!handleContainerOperation(container, true) || !moveTo(container->pos) || !robot_->takeOut(target_item->id, container->id) || !robot_->putDown(target_item->id)) return false;
        // 打开容器→移动到容器位置→取出物品→放下物品
        updateItemPosition(target_item->id, container->pos);  // 更新物品位置 容器位置
        updateItemContainer(target_item->id, 0);              // 清除物品和容器的相对位置
        success = true;
    } else if (type == "open" || type == "close") {
        if (!target_item) return false;              // 容器存在
        bool target_state = (type == "open");        // 目标状态，open为true，close为false
        if (target_item->is_open == target_state) {  // 检查当前状态，如已是目标状态则直接成功
            success = true;
        } else {  // 否则执行开关操作
            success = handleContainerOperation(target_item, target_state);
        }
    }

    return success;  // 任务执行成功
}