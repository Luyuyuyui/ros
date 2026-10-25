#!/usr/bin/env python
# -*- coding: utf-8 -*-
import rospy
import actionlib
import time
from solve_t3.msg import ELEVAction, ELEVFeedback, ELEVResult

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
        # 启动服务器
        self.server.start()
        rospy.loginfo("Goal accepted by server...")

    def execute_callback(self, goal):
        """
            执行回调函数
        """
        # 初始化结果和反馈
        result = ELEVResult()
        feedback = ELEVFeedback()
        
        start_floor = goal.current_id
        direction = goal.direction

        # 校验：当前楼层范围
        if start_floor < 1 or start_floor > 10:
            rospy.logwarn(f"Invalid floor range: current_id={start_floor} (valid: 1..10)")
            result.success = False
            self.server.set_succeeded(result)
            return
        
        # 根据方向移动一层
        if direction == "UP":
            if start_floor >= 10:
                rospy.logwarn("Cannot move UP from top floor")
                result.success = False
                self.server.set_succeeded(result)
                return
            end_floor = start_floor + 1
        elif direction == "DOWN":
            if start_floor <= 1:
                rospy.logwarn("Cannot move DOWN from bottom floor")
                result.success = False
                self.server.set_succeeded(result)
                return
            end_floor = start_floor - 1
        else:  # 非法输入
            rospy.logwarn(f"Invalid direction: {direction}")
            result.success = False
            self.server.set_succeeded(result)
            return

        # 模拟运行
        time.sleep(0.5)
        feedback.now_floor = end_floor # 更新当前楼层反馈
        self.server.publish_feedback(feedback)# 发布反馈
        
        rospy.loginfo(f"[Move Step] Elevator arrived at Floor {end_floor} | Direction: {direction}")
        
        result.success = True
        self.server.set_succeeded(result)

        

if __name__ == '__main__':
    rospy.init_node('ELEV_action_server')
    server = ELEVActionServer()
    rospy.spin()
