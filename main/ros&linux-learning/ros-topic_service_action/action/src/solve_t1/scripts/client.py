#!/usr/bin/env python
# -*- coding: utf-8 -*-
import rospy
import actionlib
from actionlib import GoalStatus
from solve_t1.msg import ELEVAction, ELEVGoal


class ELEVActionClient:
    """
    电梯Action客户端
    """

    def __init__(self):
        """
        初始化Action客户端并等待服务器连接
        """
        self.client = actionlib.SimpleActionClient('ELEV_action', ELEVAction)
        rospy.loginfo("waiting for server...")
        # 等待服务器连接，超时5秒,失败则退出
        if not self.client.wait_for_server(rospy.Duration(5.0)):
            rospy.logerr("Failed to connect to server!")
            return
            
        rospy.loginfo("Already connect Server...")

    def send_goal(self, current_id, target_id):
        """
        按下电梯发送楼层消息
        Args:
            current_id : 当前楼层
            target_id : 目标楼层
        """
        goal = ELEVGoal()# 创建目标对象
        
        # 设置电梯参数
        goal.current_id = current_id
        goal.target_id = target_id
        
        # 记录本次任务信息供回调使用
        self.current_id = current_id
        self.target_id = target_id
        self.start_time = rospy.get_time()

        # 发送目标，指定回调函数
        self.client.send_goal(
            goal,
            done_cb=self.done_callback,
            feedback_cb=self.feedback_callback
        )
        
    def feedback_callback(self, feedback):
        rospy.loginfo(f"Floor: {feedback.now_floor}")

    def done_callback(self, state, result):
        """任务完成时调用"""
        #如果任务成功完成
        if state == GoalStatus.SUCCEEDED:
            if result.success:
                duration = rospy.get_time() - self.start_time
                rospy.loginfo(f"Successfully dropped off at floor {self.target_id} (pickup from {self.current_id})\n"
                              f"Target floor: {self.target_id}  \n"
                              f"Total time: {duration:.1f}s")
            else:
                rospy.loginfo(f"Error")
        else:
            rospy.logwarn(f"Elevator task failed.")



def test_ELEV_task():
    """测试电梯任务"""
    rospy.init_node('ELEV_action_client')
    client = ELEVActionClient()
    
    rospy.sleep(1)

    current_id = 1 # 设置起始楼层是一楼
    targets = [6]
    
    rospy.loginfo(f"=== Total tasks:({len(targets)} groups) ===")

    for i, target_id in enumerate(targets):
        if rospy.is_shutdown():
            break
            
        rospy.loginfo(f"\n--- Task {i+1}: Target floor {target_id} ---")
        
        if target_id == current_id:
            rospy.loginfo("Target floor is the same as the current floor, no need to move")
            continue
            
        # 发送
        client.send_goal(
            current_id=current_id,
            target_id=target_id
        )
        
        # 等待结果
        client.client.wait_for_result()

        # 业务层结果：成功才更新当前位置
        result = client.client.get_result()
        if not result or not result.success:
            rospy.logwarn("Task finished but business failed (result.success == False)")
            rospy.sleep(1.0)
            continue
        
        # 更新当前位置为本次终点
        current_id = target_id
        rospy.sleep(1.0) # Simulate task interval

    rospy.loginfo("\n=== All tasks completed ===")

if __name__ == '__main__':
    test_ELEV_task()
