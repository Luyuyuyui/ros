#!/usr/bin/env python
# -*- coding: utf-8 -*-
import rospy
import actionlib
import time
from solve_t2.msg import ELEVAction, ELEVFeedback, ELEVResult

class ELEVActionServer:
    """
    电梯Action服务器
    """
    def __init__(self):
        # 创建Action服务器
        self.server = actionlib.SimpleActionServer(
            'ELEV_action', 
            ELEVAction, 
            self.execute_callback, 
            False
        )
        self.server.start()
        rospy.loginfo("Goal accepted by server...")

    def execute_callback(self, goal):
        # 初始化结果和反馈
        result = ELEVResult()
        feedback = ELEVFeedback()

        # 校验楼层范围
        if goal.current_id < 1 or goal.current_id > 10 or goal.target_id < 1 or goal.target_id > 10:
            rospy.logwarn(
                f"Invalid floor range.(valid: 1--10)"
            )
            result.success = False
            self.server.set_succeeded(result)
            return

        # 计算方向
        if goal.target_id > goal.current_id:
            dir = "UP"
        elif goal.target_id < goal.current_id:
            dir = "DOWN"
        else:   
            dir = "HOLD"

        rospy.loginfo(f"Elevator at Floor {goal.current_id}, To Floor {goal.target_id}, Direction: {dir}")
        # 获取起始和目标楼层
        start_floor = goal.current_id
        end_floor = goal.target_id
        
        # 确定步长
        step = 1 if end_floor > start_floor else -1
       
        # 如果起始楼层和目标楼层相同，直接返回成功
        if start_floor == end_floor:
            feedback.now_floor = end_floor
           
            self.server.publish_feedback(feedback)
            result.success = True
            self.server.set_succeeded(result)
            return
        
        current_floor = start_floor
        
        # 模拟电梯运行过程
        while current_floor != end_floor:

            time.sleep(0.5) # 模拟每层运行时间
            
            current_floor += step
            
            feedback.now_floor = current_floor
            
            self.server.publish_feedback(feedback)
            rospy.loginfo(f"[Move Step] Elevator arrived at Floor {current_floor} | Direction: {dir}")
            
        rospy.loginfo("Opening/Closing doors...")
        time.sleep(1.0) # 模拟开关门时间

        result.success = True
        self.server.set_succeeded(result)

if __name__ == '__main__':
    rospy.init_node('ELEV_action_server')
    server = ELEVActionServer()
    rospy.spin()
