// task_checker.cpp
#include "devil.hpp"
#include <iostream>
/**
 * @brief 检查当前任务是否完成
 * @param task_info 任务信息，包含任务类型和特征
 * @return false 任务未完成
 */
bool Devil::isTaskCompleted(const TaskInfo& task_info) {
    if (task_info.type == "giveme") {
        const ItemInfo* item = getItemInfo(task_info.feature);     // 调用成员函数，根据特征查找物品
        return item && item->pos == 0 && item->container_id == 0;  // 指针非空（物品存在）且物品在默认位置（手里）且物品不在容器内
        // pos == 0   表示物品在手中
    } else if (task_info.type == "puton") {  // 放到某物上
        const ItemInfo* small_item = getItemInfo(task_info.feature);
        if (!small_item) return false;

        const ItemInfo* big_item = nullptr;  // 初始化，接下来遍历找大物品
        for (const auto& pair : item_) {
            if (pair.second.name == task_info.feature.at("Y") && pair.second.type == "big") {  // 当前遍历的名字=任务中Y的值，并且类型是大物品
                big_item = &pair.second;
                break;
            }
        }
        return small_item && big_item && small_item->pos == big_item->pos && small_item->container_id == 0;
        // 任务完成：小物品存在+大物品存在+位置相同+不在容器内

    } else if (task_info.type == "putin") {
        const ItemInfo* small_item = getItemInfo(task_info.feature);
        if (!small_item) return false;

        const ItemInfo* container = nullptr;  // 初始化，接下来遍历找容器
        for (const auto& pair : item_) {
            if (pair.second.name == task_info.feature.at("Y") && pair.second.type == "container") {  // 当前遍历的名字=任务中Y的值，并且类型是容器
                container = &pair.second;
                break;
            }
        }
        return small_item && container && small_item->container_id == container->id;
        // 任务完成：小物品存在+容器存在+位置相同
    } else if (task_info.type == "takeout") {
        const ItemInfo* small_item = getItemInfo(task_info.feature);
        return small_item && small_item->container_id == 0;  // 任务完成：小物品存在+小物品在手里

    } else if (task_info.type == "open") {
        const ItemInfo* container = getItemInfo(task_info.feature);
        return container && container->is_open;  // 任务完成：容器存在+容器打开

    } else if (task_info.type == "close") {
        const ItemInfo* container = getItemInfo(task_info.feature);
        return container && !container->is_open;  // 任务完成：容器存在+容器关闭
    }

    return false;
}