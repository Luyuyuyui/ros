#!/usr/bin/env python
# -*- coding: utf-8 -*-
import rospy
import actionlib
import random
from actionlib import GoalStatus
from solve_t3.msg import ELEVAction, ELEVGoal

class ELEVActionClient:
    """
    电梯Action客户端
    """
    def __init__(self):
        """
        初始化Action客户端并等待服务器连接
        """
        self.client = actionlib.SimpleActionClient('ELEV_action', ELEVAction)
        rospy.loginfo("Waiting for server...")
        # 等待服务器连接，超时5秒,失败则退出
        if not self.client.wait_for_server(rospy.Duration(5.0)):
            rospy.logerr("Failed to connect to server!")
            return
            
        rospy.loginfo("Connected to server")

    def send_goal(self, current_id, direction):
        """
        发送电梯移动目标
        Args:
            current_id : 当前所在楼层
            direction : 移动方向 
        """
        goal = ELEVGoal()
        goal.current_id = current_id
        goal.direction = direction

        # 发送目标，并指定回调函数处理完成
        self.client.send_goal(
            goal,
            done_cb=self.done_callback
        )
        
    def done_callback(self, state, result):
        """
        任务完成回调：当Action任务失败时调用输出
        """
        if state == GoalStatus.SUCCEEDED:
            if result.success:
                pass
            else:
                # 逻辑上的失败（如电梯故障）
                rospy.logwarn(f"Error")
        else:
            # Action通信层面的异常
            rospy.logwarn(f"Action failed")


class SCAN:
    """
    电梯调度控制器,测试
    """
    def __init__(self, start_floor=1):
        """
        初始化
        """
        self.current_floor = start_floor
        self.direction = "UP"  # 初始方向向上
        self.stops = set()  # 存储所有需要停靠的楼层
        self.passengers = [] # 存储所有乘客请求 [{'start': 1, 'target': 5, 'status': 'waiting'}]
        self.client = ELEVActionClient() # 实例化Action客户端
        self.req_cnt = 0 # 乘客请求编号计数器

    def add_request(self, cnt):
        """
        生成随机乘客请求 
        Args:
            cnt: 生成的数量
        """
        for _ in range(cnt):
            self.req_cnt += 1   # 乘客请求编号自增
            start = random.randint(1, 10)# 乘客起始楼层
            
            # 生成随机方向
            if start == 1:
                req_dir = "UP"
            elif start == 10:
                req_dir = "DOWN"
            else:
                req_dir = random.choice(["UP", "DOWN"])

            # 创建乘客请求字典
            # status: 'waiting' , 'on' , 'off' 
            # target 接客后生成
            p = {'id': self.req_cnt, 'start': start, 'target': None, 'req_dir': req_dir, 'status': 'waiting'}
            self.passengers.append(p)
            
            # 在起点停靠接乘客
            self.stops.add(start)
            rospy.loginfo(f"[Goal CB]  New Request {p['id']} : Floor {start} -> goes {req_dir}")
            rospy.loginfo(f"[Goal CB] Accepted! Queue count: {len(self.passengers)}")

    def get_next_target(self):
        """
        使用 SCAN 算法计算下一个目标停靠楼层
        电梯保持当前方向运行，直到该方向无请求，然后反转方向
        """
        if not self.stops:
            return None
        
        # 将所有停靠站排序
        sorted_stops = sorted(list(self.stops))
        # 计算下一个目标楼层
        next_stops = []
        if self.direction == "UP": 
            # 筛选出当前楼层上方的所有停靠点
            for f in sorted_stops:
                if f >= self.current_floor:
                    next_stops.append(f)
            
            if not next_stops: # 如果上方没有停靠点了, 反转方向向下
                self.direction = "DOWN"
                # 筛选下方的所有停靠点
                for f in sorted_stops:
                    if f <= self.current_floor:
                        next_stops.append(f)
        
        else:
            # 筛选出当前楼层下方的所有停靠点
            for f in sorted_stops:
                if f <= self.current_floor:
                    next_stops.append(f)
            
            if not next_stops: # 如果下方没有停靠点了, 反转方向向上
                self.direction = "UP"
                # 筛选上方的所有停靠点
                for f in sorted_stops:
                    if f >= self.current_floor:
                        next_stops.append(f)
        
        # 处理异常
        if not next_stops: 
             return sorted_stops[0]

        # 返回距离最近的一个候选楼层
        target_floor = None
        if self.direction == "UP":
            target_floor = min(next_stops) # 向上找最近的
        else:
            target_floor = max(next_stops) # 向下找最近的

        return target_floor

    def run(self):
        """
        主控制循环
        """
        total_cnt = 8 # 共要处理的请求数
        
        #  初始生成的请求数量
        req = 1
        self.add_request(req)

        # 当还有停靠点需要去，或者还没生成够8个请求时，循环继续
        while self.stops or req < total_cnt:
            if rospy.is_shutdown():
                break

            # 运行过程中有概率产生新请求
            if req < total_cnt and random.random() < 0.5:
                 self.add_request(1) # 生成一个新请求
                 req += 1 # 请求数量自增

            # 计算下一个目标
            next_floor = self.get_next_target()
            
            if next_floor is None:
                continue

            # 如果已经在目标层，直接处理上下客
            if next_floor == self.current_floor:
                self.process_floor(next_floor)
                self.stops.remove(next_floor) # 从停靠集合中移除
                continue
            
            # 确定移动一步的方向
            if next_floor > self.current_floor:
                move_dir = "UP" 
            else:
                move_dir = "DOWN"

            # 发送只包含方向的目标
            self.client.send_goal(self.current_floor, move_dir)
            self.client.client.wait_for_result() # 阻塞等待到达

            # 失败则不更新楼层
            result = self.client.client.get_result()
            if not result or not result.success:
                rospy.logwarn("Move finished but business failed (result.success == False)")
                continue
            
            # 更新状态 
            if move_dir == "UP":
                self.current_floor += 1
            elif move_dir == "DOWN":
                self.current_floor -= 1

    def process_floor(self, floor):
        """
        处理每一层的上下客逻辑，实现接到乘客后生成目标楼层
        """
        open = False # 标记是否有上下客操作

        # PICKUP
        # 遍历所有乘客
        for p in self.passengers:
            # 如果乘客在该楼层等待
            if p['start'] == floor and p['status'] == 'waiting':
                #电梯门
                open = True
                # 接到乘客生成目标楼层 
                if p['req_dir'] == "UP":
                    p['target'] = random.randint(floor + 1, 10)
                else:
                    p['target'] = random.randint(1, floor - 1)
                
                p['status'] = 'on' # 状态变为在电梯上
                self.stops.add(p['target']) # 将乘客的目的地加入集合
                # 计算当前电梯内人数
                people = 0
                for pp in self.passengers:
                    if pp['status'] == 'on':
                        people += 1
                rospy.loginfo(f"[Finish PICKUP]  PICKUP at {floor} done.(target: {p['target']}) | Passagers: {people}")
        
        # DROPOFF
        # 遍历所有乘客
        for p in self.passengers:
            if p['target'] == floor and p['status'] == 'on':
                p['status'] = 'off' # 状态变为完成
                open = True
                # 计算当前电梯内人数
                people = 0
                for pp in self.passengers:
                    if pp['status'] == 'on':
                        people += 1
                rospy.loginfo(f"[Finish DROPOFF] DROPOFF at {floor} done. | Passagers: {people}")
        
        # 模拟开关门时间 1s
        if open:
            rospy.loginfo("Opening/Closing doors...")
            rospy.sleep(1.0) 


def test_ELEV_task():
    """主测试函数"""
    # 初始化ROS节点
    rospy.init_node('ELEV_action_client')
    rospy.sleep(1)

    # 随机初始位置
    start_floor = 1
    rospy.loginfo(f"=== Task Ready (initial floor: {start_floor} ) ===")
    
    controller = SCAN(start_floor)
    controller.run() # 开始运行调度循环

    rospy.loginfo("\n=== All tasks completed ===")

if __name__ == '__main__':
    test_ELEV_task()
