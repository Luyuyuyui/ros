/**
 * @file robot.hpp
 * @brief Robot类的定义
 */
#pragma once
#ifndef ROBOT_HPP
#define ROBOT_HPP
#include <memory>
#include <string>

class Robot {
 public:
  /**
   * @brief 通过环境信息字符串进行构造
   * @param env_str 环境信息字符串
   * @note 该构造函数不会对环境信息字符串的语法和所含信息是否正确进行检测。@n
   *       调用者应确保环境信息字符串的正确性
   */
  explicit Robot(const std::string& env_str);
  ~Robot();
  // 不允许复制
  Robot(const Robot&) = delete;
  Robot& operator=(const Robot&) = delete;
  // 允许移动
  Robot& operator=(Robot&& rhs) noexcept;
  Robot(Robot&& rhs) noexcept;

  /**
   * @brief 让机器人移动到目标位置。
   * @param loc 目标位置
   * @return 如果执行成功返回true,
   * 如果位置编号没有出现在环境信息中或已处于目标位置则返回false
   *
   * @details 如果成功移动则打印[Move loc | True], 失败则打印
   * [Move loc | False]。(loc会被替换为目标位置)@n
   * 并且如果移动失败则打印失败原因Reason:...。失败原因会被*****符号上下包围。
   */
  bool move(std::size_t loc) const noexcept;
  /**
   * @brief 让机器人拿取指定小物品
   * @note
   * 使用该函数前需要机器人与小物品处于同一位置，且小物品不处于容器内，且机器人手上没有小物品。@n
   * 若是需要从容器内拿取物品则使用takeOut()函数。
   * @param id_small_item 物品id
   * @return 如果执行成功返回true, 否则false
   *
   * @details 如果成功拿取则打印[Pick id Up | True],
   * 失败则打印[Pick id Up |False]。(id会被替换为目标小物品id)@n
   * 并且如果拿取失败则打印失败原因Reason:...。失败原因会被*****符号上下包围。
   */
  bool pickUp(std::size_t id_small_item) const noexcept;

  /**
   * @brief 让机器人从指定容器内拿取小物品
   * @note
   * 使用该函数前需要机器人与容器处于同一位置，且小物品处于该容器内，且机器人手上没有小物品，且容器为开启状态。
   * @param id_small_item 小物品id
   * @param id_container 容器id
   * @return 如果执行成功返回true, 否则false
   *
   * @details 如果成功拿取则打印[Take id_s Out From id_c | True],
   * 失败则打印[Take id_s Out From id_c | False]。(id_s会被替换为目标小物品id,
   * id_c会被替换为容器id)@n
   * 并且如果拿取失败则打印失败原因Reason:...。失败原因会被*****符号上下包围。
   */
  bool takeOut(std::size_t id_small_item,
               std::size_t id_container) const noexcept;

  /**
   * @brief 让机器人放下指定物品
   * @note 使用该函数前需要机器人持有该物品。并且不会放到容器内部。
   * @param id_small_item 小物品id
   * @return 如果执行成功返回true, 否则false
   *
   * @details 如果成功放下则打印[Put id Down | True], 失败则打印
   * [Put id Down | False]。(id会被替换为目标小物品id)@n
   * 并且如果放下失败则打印失败原因Reason:...。失败原因会被*****符号上下包围。
   */
  bool putDown(std::size_t id_small_item) const noexcept;

  /**
   * @brief 让机器人将小物品放到指定容器内部
   * @note
   * 使用该函数前需要机器人与容器处于同一位置，且机器人持有该小物品，且该容器为开启状态
   * @param id_small_item 小物品id
   * @param id_container 容器id
   * @return 如果执行成功返回true, 否则false
   *
   * @details 如果成功放入则打印[Put id_s in id_c | True],
   * 失败则打印[Put id_s in id_c | False]。(id_s会被替换为目标小物品id,
   * id_c会被替换为目标容器id)@n
   * 并且如果拿取失败则打印失败原因Reason:...。失败原因会被*****符号上下包围。
   */
  bool putIn(std::size_t id_small_item,
             std::size_t id_container) const noexcept;

  /**
   * @brief 让机器人打开指定容器
   * @note
   * 使用该函数前需要机器人与容器处于同一位置，且机器人不持有任何小物品，且该容器为关闭状态
   * @param id_container 容器id
   * @return 如果执行成功返回true, 否则false
   *
   * @details 如果成功打开则打印[Open id | True], 失败则打印[Open id |
   * False]。(id会被替换为目标容器id)@n
   * 并且如果拿取失败则打印失败原因Reason:...。失败原因会被*****符号上下包围。
   */
  bool open(std::size_t id_container) const noexcept;

  /**
   * @brief 让机器人关闭指定容器
   * @note
   * 使用该函数前需要机器人与容器处于同一位置，且机器人不持有任何小物品，且该容器为打开状态
   * @param id_container 容器id
   * @return 如果执行成功返回true, 否则false
   *
   * @details 如果成功打开则打印[Close id | True], 失败则打印[Close id |
   * False]。(id会被替换为目标容器id)@n
   * 并且如果拿取失败则打印失败原因Reason:...。失败原因会被*****符号上下包围。
   */
  bool close(std::size_t id_container) const noexcept;

 private:
  struct Impl;
  std::unique_ptr<Impl> m_impl;  // 指向实现的指针
};

#endif  // ROBOT_HPP
