# ROS Action 下半考核 通信
初步了解 action 通信,完成电梯调度问题

## 任务点一
### 题目要求
请利用action通信，模拟一次翱翔屠鼠馆（1-10层）电梯接送客的过程。
电梯初始状态在1楼，现在在x楼的某乘客按了一下电梯，你可以得到的信息是乘客的当前楼层和目标楼层,你所做的是利用action通信的特性，来对电梯所在楼层进行实时反馈，电梯上下行的速度为0.5s/楼，开,关门时间为1s,停留时间简化为0
### 思路
输入：乘客的当前楼层 `current_id` 与目标楼层 `target_id`
输出：实时反馈当前楼层
客户端发 Goal；服务端执行并周期性发 Feedback；执行结束发 Result
一次调度
+ action：客户端给服务器发当前楼层和目标楼层，服务器返回是否成功运行，实时返回到达的楼层
	- Goal：`current_id`、`target_id`
	- Feedback：`now_floor`
	- Result：`success`
+ 服务端：比较当前和目标楼层计算方向，确定步长的正负
	- `execute_callback`计算运行参数，循环发布反馈 `publish_feedback`，结束调用 `set_succeeded`
+ 客户端：发送当前楼层和目标楼层

调用链：客户端 `send_goal()` 发起 → 服务端 `execute_callback()` 执行 → `publish_feedback()` 触发客户端 `feedback_callback()` → `set_succeeded()` 触发客户端 `done_callback()`。


```python
test_ELEV_task()
├─ rospy.init_node
├─ ELEVActionClient.__init__()
│  └─ wait_for_server
├─ ELEVActionClient.send_goal(current_id, target_id)
├─ feedback_callback(feedback)   ← 被 Server.publish_feedback(...) 触发
└─ done_callback(state, result)  ← 被 Server.set_succeeded(...) 触发

Server
__main__
├─ rospy.init_node
├─ ELEVActionServer.__init__()
│  └─ server.start()
└─ execute_callback(goal)  ← 被客户端 send_goal 发送的 goal 触发
   ├─ server.publish_feedback
   └─ server.set_succeeded(result)     
```

## 任务二
### 思路
8次调度，需要处理多条乘客请求，且运行过程中会插入新请求，需要函数负责决定下一步去哪个楼层
+ action 同上：Goal/Feedback/Result。
+ 客户端不再只发一次 goal，而是在循环中多次调用 `send_goal`
调度封装为 `SCAN` 类，对外只有 `run()` 作为主入口
用 `passengers` 保存请求与状态，用 `stops` 保存所有需要停靠的楼层。
设计增加请求函数`add_request()`负责产生请求并更新停靠列表，随机增加请求通过if+概率实现
设计决定下一层停靠函数`get_next_target()`选择下一站：scan算法
设计到达后更新乘客状态函数`process_floor()`（默认每一层只有一人）
处理电梯停靠流程`run()`，当请求不够8个或停靠点不空时执行；得到下一目标层，递归处理，更新状态；每次完成停靠后在停靠列表中删除楼层，更新乘客状态，调用上面三个函数
+ 服务器同上


调用链总结
  - `test_ELEV_task()` → `SCAN.run()`
  - `SCAN.run()` 递归循环：`add_request()` / `get_next_target()` → `ELEVActionClient.send_goal()` → `wait_for_result()` → `get_result()` → `process_floor()`

```python
test_ELEV_task()
├─ rospy.init_node(...)
├─ SCAN.__init__(start_floor)
│  └─ ELEVActionClient.__init__()
│     └─ wait_for_server(...)
└─ SCAN.run()
   ├─ add_request(cnt)
   ├─ get_next_target()（每轮循环决策下一站）
   ├─ ELEVActionClient.send_goal(current_floor, next_floor)
   └─ process_floor(next_floor)

__main__
├─ rospy.init_node(...)
├─ ELEVActionServer.__init__()
│  └─ server.start()
└─ execute_callback(goal)
   ├─ (循环内多次) server.publish_feedback(feedback)
   └─ server.set_succeeded(result) 
```

## 任务三
### 思路
请求发出时只有起始楼层 + 方向，目标楼层要等到上客后才能确定,Goal 只包含 `current_id + direction`
服务端不知道停止位置，只能确定下一步，通过多次调用服务端实现
+ action：客户端给服务器发当前楼层和目标楼层，服务器返回是否成功运行，实时返回到达的楼层
	- Goal：`current_id`、`direction`
	- Feedback：`now_floor`
	- Result：`success`
+ 客户端：
请求在 `add_request()` 设置 `target=None`
目标的随机生成在 `process_floor()` 的 PICKUP 分支，当乘客从 waiting→on 时，根据目标方向和初始楼层设置随机数范围生成目标楼层
`run()` 负责：
  - 不在目标层时：计算 `move_dir`，调用 `send_goal`（走一步） 更新 `current_floor`。
  - 到达需要停靠的楼层时：调用 `process_floor()` 生成/处理目标并维护 `stops`。
其余同t2
+ 服务器：一步一步执行，其余同t2

调用链总结
- `test_ELEV_task()` → `SCAN.run()`
- `SCAN.run()` 循环：`add_request()` / `get_next_target()` → (多次) `ELEVActionClient.send_goal(step)` → `wait_for_result()` → `get_result()` → (到站) `process_floor()`


```python
test_ELEV_task()
├─ rospy.init_node(...)
├─ SCAN.__init__(start_floor)
│  └─ ELEVActionClient.__init__()
│     └─ wait_for_server(...)
└─ SCAN.run()
   ├─ add_request(cnt)
   ├─ get_next_target()
   ├─ (未到目标层 )
   │  ├─ 计算 move_dir
   │  ├─ ELEVActionClient.send_goal(current_floor, move_dir)
   └─ process_floor(floor)
      └─ （PICKUP 分支）写入 p['target']

Server
__main__
├─ rospy.init_node(...)
├─ ELEVActionServer.__init__()
│  └─ server.start()
└─ execute_callback(goal)（一次 goal 只走一层）
   ├─ server.publish_feedback(feedback) 
   └─ server.set_succeeded(result) 
```
## 学习笔记

### Action通讯
action是一种类似于Service的问答通信机制，不同之处在于action带有连续反馈，可以不断反馈任务进度，也可以在任务过程中中止运行。
采用服务器/客户端（Client/Server）的工作模式
Client向Server端发布任务目标以及在必要的时候取消任务，Server会向Client发布当前状态、实时反馈和任务执行的最终结果。

-（1）goal：发布任务目标；
-（2）cancel：请求取消任务；
-（3）status：通知Client当前的状态；
-（4）feedback：周期反馈任务运行的监控数据；
-（5）result：向Client发送任务的执行结果，只发布一次。

> .action文件
```
# 定义目标信息
uint32 dishwasher_id # Specify which dishwasher we want to use
---
# 定义结果信息
uint32 total_dishes_cleaned
---
# 定义周期反馈的消息
float32 percent_complete
```
### 电梯调度
#### SCAN
如高层写字楼早高峰时，电梯先向上依次搭载各楼层乘客，直至无向上请求后，再切换方向向下运行。这种模式减少无效停靠，提高运输效率。​其核心是对资源请求进行方向优先排序，以降低寻道时间或等待延迟，提升系统吞吐量与响应效率。​
`单电梯调度`
初始化：确定资源处理单元（如磁盘磁头、电梯轿厢）的初始位置与移动方向。例如，在磁盘调度场景中，磁头初始位于 50 号磁道，方向设为向上；在电梯调度中，电梯初始停靠在 1 楼，方向向上。​
同向处理：资源处理单元沿当前方向，按顺序处理请求队列中与该方向一致的请求。以磁盘调度为例，若磁头方向向上，则依次处理磁道号大于 50 的请求；电梯场景中，电梯向上依次响应高于 1 楼的楼层请求。此阶段不处理反方向请求。​
方向切换：当同向请求处理完毕，资源处理单元立即切换方向，开始处理反方向请求。如磁头处理完所有向上磁道请求后，转为向下处理剩余请求；电梯处理完上方楼层请求后，开始向下运行。​

实现：
```python
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

```
#### LOOK
头从第一个请求开始，然后移动到另一端，处理其间产生的所有请求。一旦它到达另一端的最后一个请求，头部就会反转它的方向并返回到第一个请求，处理`其间`的所有请求。
LOOK算法避免了磁头移动到磁盘两端的无效操作，减少了总移动距离。
