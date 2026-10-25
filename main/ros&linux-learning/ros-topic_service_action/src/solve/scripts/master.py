#!/usr/bin/env python3
"""master.py
主控节点（Master）：
- 订阅 voice 节点（语音）发布的任务 Topic：/task (solve/msg/match)
- 调用 vision 节点（视觉）提供的服务：/get_location (solve/srv/Location)
    - 输入：target
    - 输出：name, x, y
- 调用 move 节点（运动）提供的服务：/move (solve/srv/move)
    - 输入: x, y
    - 输出：x, y, success

任务流程：
- pick_up：移动到 ball 坐标并拿球
- dribble：移动到战术点位坐标
- shoot：不移动；在当前位置朝 hoop 投篮
"""

import rospy
from collections import deque
from solve.srv import move
from solve.srv import Location
from solve.msg import match
from geometry_msgs.msg import Point

class MasterNode:
    """
    MasterNode：主控节点，负责协调 voice/vision/move 完成任务
    """
    def __init__(self):
        rospy.init_node('master')

        # 任务总数:从launch文件获取
        self.task_num = int(rospy.get_param('~task_num'))
        self.pro_tasks = 0# 已处理的任务数
        # 任务队列：收到的任务先放这里，等到用户输入 start 后再逐条处理
        self.task_queue = deque(maxlen=50)
        # 在主控节点终端输入 start 才开始执行队列里的任务
        self.started = False
        self.cx = 0.0
        self.cy = 0.0
        # 订阅 move 节点实时的位置
        self.pose_sub = rospy.Subscriber('move_pose', Point, self.now_pose, queue_size=10)
        # 订阅task节点(语音模块)
        self.task_sub = rospy.Subscriber('task', match, self.add_task, queue_size=100)

        # 等待 vision 和 move 服务上线
        rospy.wait_for_service('get_location')
        rospy.wait_for_service('move')
        # 创建服务代理对象，更加方便地调用 vision 和 move 的服务
        self.get_location = rospy.ServiceProxy('get_location', Location)
        self.move = rospy.ServiceProxy('move', move)

    def start_task(self):
        """
         在终端等待用户输入 start，才开始执行任务队列里的任务
         """
        # 在终端等待用户输入 start
        while not rospy.is_shutdown() and not self.started:
            input_word = input("=== Type 'start' to begin tasks: === ").strip()
            if input_word == 'start':
                self.started = True
                rospy.loginfo("=== Starting to execute tasks！ ===")
                return

    def add_task(self, msg: match):
        # 收到任务后先放到队列里，等用户输入 start 后再处理
        self.task_queue.append(msg)
        
    def call_move(self, x, y):
        # 动作模块：把目标坐标发给 move
        return self.move(x, y)

    def solve_task(self, task_msg):
        # 处理单条任务：
        # 1 用 target 向 vision 查询坐标
        # 2 把坐标交给 move 执行
        rospy.loginfo(
            "Received tasks: [%s] [%s]",
            task_msg.target,
            task_msg.action,
        )

        action = task_msg.action.strip()
        target = task_msg.target.strip()

        # pick_up
        if action == "pick_up":
            loc = self.get_location(target)  # 根据 target 查询坐标
            if loc is None:
                return

            rospy.loginfo("Vision confirmed: %s is at (%.1f, %.1f)", loc.name, loc.x, loc.y)
            # 调用 move 服务移动到球的位置
            temp = self.move(loc.x, loc.y)
            if temp is None or not temp.success:
                return
            # 更新当前位置
            self.cx = temp.x
            self.cy = temp.y
            rospy.loginfo("%s the %s",action,target,)
            return

        # dribble
        if action == "dribble":
            loc = self.get_location(target)
            if loc is None:
                return
            rospy.loginfo("Vision confirmed: %s is at (%.1f, %.1f)", loc.name, loc.x, loc.y)
            # 调用 move 服务移动
            temp = self.move(loc.x, loc.y)
            if temp is None or not temp.success:
                return
            self.cx = temp.x
            self.cy = temp.y

            rospy.loginfo("%s the %s", action, target)
            return

        # shoot
        if action == "shoot":
            # shoot 
            temp = self.get_location(target)
            if temp is None:
                return

            rospy.loginfo("Vision confirmed: %s is at (%.1f, %.1f)", temp.name, temp.x, temp.y)
            rospy.loginfo("%s the %s", action, target)
            return

        return

    def run(self):
        rate = rospy.Rate(1.0)
        # 等待用户输入 start 后才开始执行任务
        if not self.started:
            self.start_task()
        while not rospy.is_shutdown():
            #队列存在
            if self.task_queue:
                # 取出一条任务并处理
                task_msg = self.task_queue.popleft()
                self.solve_task(task_msg) 

                # 统计已完成的任务数
                self.pro_tasks += 1
        
                if self.pro_tasks >= self.task_num:
                    rospy.loginfo("All tasks finished!")
                    break
            else:
                rospy.loginfo("Waiting for tasks ...")
            rate.sleep()

    def now_pose(self, msg: Point):
        # move 广播的实时坐标
        self.cx = msg.x
        self.cy = msg.y
        rospy.loginfo("Current position : (%.1f, %.1f)", self.cx, self.cy)

if __name__ == '__main__':
    node = MasterNode()
    node.run()