#!/usr/bin/env python3
import math
import rospy
from solve.srv import move, moveResponse
from geometry_msgs.msg import Point # 用于发布当前位置

class MoveServer:
    """
    MoveServer：运动节点，执行移动并实时发布当前位置
    """
    def __init__(self):
        rospy.init_node('move')
        # 创建服务,名称为 move,类型为 move,回调函数为 solve_req
        self.service = rospy.Service('move', move, self.solve_req)
        # 发布当前位置给 master 节点
        self.pos = rospy.Publisher('move_pose', Point, queue_size=10)
        # 用于浮点数比较
        self.compare = 0.05
        # 默认起点
        self.x = 0.0
        self.y = 0.0

    def pub_pose(self):
        # 把当前位置发布到 move_pose
        p = Point()
        p.x = self.x
        p.y = self.y
        self.pos.publish(p)
        
    
    def solve_req(self, req):
        """
         处理 move 服务请求，移动到 req.x, req.y
         实时发布当前位置"""
        # 收到目标点后开始移动
        goal_x = float(req.x)
        goal_y = float(req.y)

        res = moveResponse()# 创建响应对象，准备填充返回值

        rospy.loginfo("Moving to (%.1f, %.1f), currently at (%.1f, %.1f)", goal_x, goal_y, self.x, self.y)
    
        # 浮点数判断是否到达：通过计算当前点与目标点的距离 dist 和 compare 
        dx = goal_x - self.x
        dy = goal_y - self.y
        dist = math.hypot(dx, dy) # 计算当前点与目标点的欧几里德距离

        #没有到达
        while (not rospy.is_shutdown()) and (dist > self.compare):
            # 如果距离小于等于1，直接落到目标点并返回成功
            if dist <= 1.0:
                self.x = goal_x
                self.y = goal_y
                self.pub_pose()
                res.x = self.x
                res.y = self.y
                res.success = True
                rospy.loginfo("Arrived at (%.1f, %.1f)", self.x, self.y)
                return res

            # 沿着目标方向走一步
            if dist > 0.0:
                self.x += 1.0 * (dx / dist) #dx/dist 和 dy/dist 是单位向量，乘以 step_size 就是每次移动的增量
                self.y += 1.0 * (dy / dist)

            # 实时广播当前位置给 master
            self.pub_pose()
           
            rospy.sleep(1.0)

            # 更新距离
            dx = goal_x - self.x
            dy = goal_y - self.y
            dist = math.hypot(dx, dy)

        # 最后到目标点
        if not rospy.is_shutdown():
            self.x = goal_x
            self.y = goal_y
            self.pub_pose()
            res.x = self.x
            res.y = self.y
            res.success = True
            rospy.loginfo("Arrived at (%.1f, %.1f)", self.x, self.y)
            return res

        #失败
        res.x = self.x 
        res.y = self.y
        res.success = False
        return res
    
    def run(self):
        rospy.spin()

if __name__ == '__main__':
    server = MoveServer()
    server.run()