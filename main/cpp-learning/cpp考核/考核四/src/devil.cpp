#include "devil.hpp"
#include <iostream>
#include <algorithm>
#include <regex>
/**
 * @brief 构造函数
 * @param env_str 环境描述字符串,初始环境状态信息
 *
 * @details
 */
Devil::Devil(const std::string& env_str) : robot_(std::unique_ptr<Robot>(new Robot(env_str))) {
    parseEnvironment(env_str);       // 解析环境信息
    updateContainerItemPositions();  // 建立容器关系，同步容器和物品的位置
}
/**
 * @brief 移动构造函数
 * @param rhs 要移动的右值对象
 * @details 避免深拷贝，只转移指针
 */
Devil::Devil(Devil&& rhs) noexcept
    : robot_(std::move(rhs.robot_)), item_(std::move(rhs.item_)), task_(std::move(rhs.task_)), task_order(std::move(rhs.task_order)), task_completed(std::move(rhs.task_completed)) {}
/**
 * @brief 移动赋值运算符
 * @param rhs 要移动的右值对象
 * @return Devil& 返回当前对象的引用，使得支持链式移动赋值devil1 = devil2 =
 * std::move(devil3);
 */
Devil& Devil::operator=(Devil&& rhs) noexcept {
    if (this != &rhs) {  // 检查是否是自赋值
                         // 不是自赋值，安全执行移动
        robot_ = std::move(rhs.robot_);
        item_ = std::move(rhs.item_);
        task_ = std::move(rhs.task_);
        task_order = std::move(rhs.task_order);
        task_completed = std::move(rhs.task_completed);
    }
    return *this;
}
/**
 * @brief 读取存储解析任务
 * @param task_str 任务描述字符串
 * 实现任务的存储并且初始化
 */
void Devil::readTask(const std::string& task_str) {
    auto task = parseTask(task_str);
    // 调用解析任务函数实现任务解析，收到任务ID到任务信息的映射
    // 清空现有任务信息
    // task_ ：std::map<size_t, TaskInfo>
    task_.clear();
    task_order.clear();
    task_completed.clear();
    // 存储新解析的任务
    task_ = std::move(task);
    for (const auto& pair : task_) {         // pair 包含两个成员
        task_order.push_back(pair.first);    // 任务ID
        task_completed[pair.first] = false;  // 标记任务还未完成
    }
}
/**
 * @brief 完成所有任务
 * 按任务顺序依次执行所有未完成的任务
 */
void Devil::Complete() {
    for (const auto& pair : task_) {
        Complete(pair.first);
    }
}
/**
 * @brief 完成指定ID的任务
 * 执行指定ID的任务，如果任务已完成或不存在则跳过
 * @param id 任务ID
 */
void Devil::Complete(size_t id) {
    auto it = task_.find(id);  // 找到通过readTask解析得到的任务ID
    if (it == task_.end()) {   // 如果已经完成了所有任务
        return;
    }
    // 检查任务执行记录（在这次执行前已经被执行过）
    if (task_completed[id]) {
        std::cout << "<<<<<<<< Task " << id << " has completed before. >>>>>>>>" << std::endl;
        return;
    }
    const auto& task_info = it->second;
    // 找到通过readTask解析得到的任务具体信息
    // 检查任务在当前过程中是否已经完成
    if (isTaskCompleted(task_info)) {
        task_completed[id] = true;
        std::cout << "<<<<<<<< Task " << id << " is already completed. >>>>>>>>" << std::endl;
        return;
    }
    // 开始执行任务
    std::cout << "<<<<<<<< Start Complete Task " << id << " >>>>>>>>" << std::endl;
    // 检查任务执行成功与否
    bool success = executeTask(id, task_info, item_);
    if (success) {
        task_completed[id] = true;
        std::cout << "<<<<<<<< Task " << id << " completed successfully. >>>>>>>>" << std::endl;
    } else {
        std::cout << "<<<<<<<< Task " << id << " failed to complete. >>>>>>>>" << std::endl;
    }
    std::cout << "<<<<<<<< End Complete Task " << id << " >>>>>>>>" << std::endl;
}
/**
 * @brief 解析环境字符串
 * @param env_str 环境描述字符串
 * 将信息按行切割，提取每一行并传送到行解析函数
 */
void Devil::parseEnvironment(const std::string& env_str) {
    size_t pos = 0;  // 用于分割行
    size_t len = env_str.length();
    while (pos < len) {
        size_t line_end = env_str.find('\n', pos);
        if (line_end == std::string::npos) {  // 如果只有一行
            line_end = len;
        }
        // 提取每一行信息
        std::string line = env_str.substr(pos, line_end - pos);
        // 如果行内容非空，则进行解析
        if (!line.empty()) {
            parseEnvironmentLine(line);
        }
        pos = line_end + 1;  // 切分下一行内容
    }
}
/**
 * @brief 解析单行环境信息
 * 用正则表达式解析物品的属性信息
 * @param line 单行环境描述字符串
 */
void Devil::parseEnvironmentLine(const std::string& line) {
    ItemInfo item;  // 创建物品并初始化
    item.container_id = 0;
    item.is_open = false;
    // 创建正则表达式
    std::string pattern(R"(\((\w+)(?:\s+([^)]+))?\))");
    std::regex r(pattern);
    // 先收集所有属性，把所有 (key value) 对提取到 attributes map 中
    std::map<std::string, std::string> attributes;  // 临时存储
    for (std::sregex_iterator it(line.begin(), line.end(), r), end; it != end; ++it) {
        std::smatch match = *it;
        std::string key = match[1];
        std::string value = match[2];
        attributes[key] = value;
    }
    // 解释(sort 1 bottle)这种属性
    if (attributes.count("sort")) {                   // 检查"sort"是否存在
        std::string sort_value = attributes["sort"];  // 获取sort属性的值
        size_t space_pos = sort_value.find(' ');      // 在值中查找空格，分离ID和名称
        if (space_pos != std::string::npos) {
            std::string id_str = sort_value.substr(0, space_pos);
            item.id = std::stoul(id_str);                  // 字符串转为无符号整数
            item.name = sort_value.substr(space_pos + 1);  // 提取名称
        }
    }
    // 解析其他属性
    if (attributes.count("type")) item.type = attributes["type"];  // 检查类型存在并且获取存储对应位置的值
    if (attributes.count("color")) item.color = attributes["color"];
    // 如果在容器内
    if (attributes.count("inside")) {
        item.container_id = std::stoul(attributes["inside"]);
        // 将 inside 属性的值转换为无符号整数，并设置为物品的容器ID
        //  查找容器的位置
        auto container_it = item_.find(item.container_id);  // 查找对应容器
        // container_it 是指向 std::pair<const size_t, ItemInfo> 的迭代器
        if (container_it != item_.end()) {        // 找到了容器
            item.pos = container_it->second.pos;  // 更新物品的位置为容器的位置
            // container_it->second → ItemInfo 对象（容器物品的信息）
            // container_it->second.pos    → ItemInfo 中的 pos成员（容器的位置）
        } else {
            // 如果容器还没解析到，先设为0，后面再更新
            item.pos = 0;
        }
    } else if (attributes.count("at")) {  // 如果在容器上
        item.pos = std::stoul(attributes["at"]);
    }
    if (attributes.count("open")) item.is_open = true;
    if (attributes.count("close")) item.is_open = false;
    // 将解析好的物品信息存储到物品字典中
    if (item.id != 0) {
        item_[item.id] = item;
    }
}
/**
 * @brief 拆分任务字符串
 * @param task_str 任务字符串
 * @return std::map<std::size_t, TaskInfo> 返回任务ID到任务信息的映射
 * 按行将任务字符串拆分，交给后续函数解析
 */
std::map<std::size_t, Devil::TaskInfo> Devil::parseTask(const std::string& task_str) {
    std::map<std::size_t, TaskInfo> task;
    // 按照行拆分任务信息
    size_t pos = 0;
    size_t len = task_str.length();
    while (pos < len) {  // 同样按行拆分
        size_t line_end = task_str.find('\n', pos);
        if (line_end == std::string::npos) {
            line_end = task_str.length();
        }
        std::string line = task_str.substr(pos, line_end - pos);
        if (!line.empty()) {
            parseTaskLine(line, task);
        }
        pos = line_end + 1;
    }
    return task;
}
/**
 * @brief 解析单行任务信息
 * 使用正则表达式解析任务ID、类型、对象和特征信息
 * @param line 单行任务描述字符串
 * @param task 任务映射表，用于存储解析结果
 */
void Devil::parseTaskLine(const std::string& line, std::map<std::size_t, TaskInfo>& task) {
    // 创建任务的正则表达式
    std::string task_pattern(R"(\(task\s+(\d+)\)\s+(.+))");
    std::regex r1(task_pattern);

    std::size_t task_id = 0;
    // 解析获取任务的id
    for (std::sregex_iterator it(line.begin(), line.end(), r1), end; it != end; ++it) {
        std::smatch task_match = *it;
        task_id = std::stoull(task_match[1]);
        break;  // 只匹配一个任务
    }
    if (task_id == 0) return;  // id不能为0

    TaskInfo task_info;
    // 创建内容的正则表达式
    std::string content_pattern(R"(\((\w+)\s+([^)]+)\))");
    std::regex r2(content_pattern);
    // 解析获取内容
    for (std::sregex_iterator it(line.begin(), line.end(), r2), end; it != end; ++it) {
        std::smatch match = *it;
        std::string key = match[1];
        std::string value = match[2];
        // 解析type
        if (key == "giveme" || key == "puton" || key == "putin" || key == "takeout" || key == "open" || key == "close") {
            task_info.type = key;  // 存储type
            // 解析对象X Y
            size_t pos = 0;
            while (pos < value.length()) {
                size_t space_pos = value.find(' ', pos);
                if (space_pos == std::string::npos) {  // 处理最后一个对象
                    task_info.object.push_back(std::make_pair("param", value.substr(pos)));
                    break;  // 处理完最后一个对象跳出循环
                } else {    // 不是最后一个，依次更新pos获取对象
                    task_info.object.push_back(std::make_pair("param", value.substr(pos, space_pos - pos)));
                    pos = space_pos + 1;
                }
            }
        } else if (key == "item") {  // 解析item   (item X bottle)
            size_t space_pos = value.find(' ');
            if (space_pos != std::string::npos) {
                std::string variable = value.substr(0, space_pos);  // 空格前，变量名
                std::string val_str = value.substr(space_pos + 1);  // 空格后：实际名字
                task_info.feature[variable] = val_str;
                // task_info.feature 现在包含：{"X": "bottle"}
            }
        } else if (key == "color") {  // 解析color  (color X blue)
            size_t space_pos = value.find(' ');
            if (space_pos != std::string::npos) {
                std::string variable = value.substr(0, space_pos);  // 空格前，变量名
                std::string val_str = value.substr(space_pos + 1);  // 空格后：实际颜色
                task_info.feature["color"] = val_str;               // 颜色特征直接存储为 "color"
                // task_info.feature 现在包含：{"color": "blue"}
                // 同时记录颜色对应的变量
                task_info.feature[variable + "_color"] = val_str;
                // task_info.feature 现在包含：{"X color": "blue"}
            }
        }
    }

    task[task_id] = task_info;  // 将任务ID与信息对应
}

/**
 * @brief 根据特征条件查找匹配的物品信息
 * @return 返回Devil 类中的
 * ItemInfo类型，指向匹配物品信息的指针，如果未找到返回nullptr
 */
const Devil::ItemInfo* Devil::getItemInfo(const std::map<std::string, std::string>& feature) const {
    // 查询条件在feature中
    //  遍历所有物品
    for (const auto& pair : item_) {
        const ItemInfo& item = pair.second;
        bool match = true;  // 标记当前物品是否匹配所有特征条件

        // 检查所有特征条件
        for (const auto& feat : feature) {
            // feat.first 是查询条件的键，如 "item", "color"
            // feat.second 是查询条件的值，如  "red"
            // 特征键为 "X" 或 "item" 时，检查物品名称
            if (feat.first == "X" || feat.first == "item") {
                // 检查物品名称
                if (item.name != feat.second) {  // 不匹配
                    match = false;
                    break;
                }
            } else if (feat.first == "color") {
                // 检查颜色
                if (item.color != feat.second) {
                    match = false;
                    break;
                }
            }
            //"Y" 是另一个物品
        }

        if (match) {
            return &item;
        }
    }
    return nullptr;  // 返回空指针
}
/**
 * @brief 更新物品位置
 * @param item_id 物品的ID
 * @param new_position 新的位置
 */
void Devil::updateItemPosition(size_t item_id, size_t new_position) {
    auto it = item_.find(item_id);  // 寻找物品
    if (it != item_.end()) {
        it->second.pos = new_position;  // 更新位置信息
    }
}
/**
 * @brief 更新物品所属的容器
 * @param item_id 物品的ID
 * @param container_id 容器ID
 */
void Devil::updateItemContainer(size_t item_id, size_t container_id) {
    auto it = item_.find(item_id);
    if (it != item_.end()) {
        it->second.container_id = container_id;
    }
}
/**
 * @brief 更新容器的开关状态
 * @param is_open 开闭状态
 * @param container_id 容器ID
 */
void Devil::updateContainerOpenState(size_t container_id, bool is_open) {
    auto it = item_.find(container_id);
    if (it != item_.end()) {
        it->second.is_open = is_open;
    }
}
/**
 * @brief 更新物品和容器的相对位置
 * 同步物品和容器的位置，确保容器移动时，物品跟着移动
 */
void Devil::updateContainerItemPositions() {
    for (auto& pair : item_) {
        ItemInfo& item = pair.second;
        if (item.container_id != 0) {
            auto container_it = item_.find(item.container_id);
            if (container_it != item_.end()) {
                item.pos = container_it->second.pos;  // 物品的位置和容器的位置同步
            }
        }
    }
}