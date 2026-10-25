ROS 的概念分为三个层次：文件系统层、计算图层、社区层

**安装和配置ROS环境**
`source /opt/ros/noetic/setup.bash`  # 临时生效
`echo "source /opt/ros/noetic/setup.bash" >> ~/.bashrc`  # 永久生效

 **ROS文件系统导览**
roscd：直接跳转到某个ROS包的目录
rosls：列出ROS包里的文件
rospack：查找ROS包的信息

**创建ROS软件包**
**创建工作空间**
`mkdir -p ~/catkin_ws/src`
`cd ~/catkin_ws/`
`catkin_make`
`source devel/setup.bash`

包包括以下：
`my_package/`
  `CMakeLists.txt`
  `package.xml`
catkin_create_pkg：创建新包的命令
定义包依赖（说明这个包需要哪些其他包的支持
`catkin_create_pkg <package_name> [depend1] [depend2] [depend3]`
 e.g.
`catkin_create_pkg beginner_tutorials std_msgs rospy roscpp`

**自定义package.xml**
描述标签: 
`<description>The beginner_tutorials package</description>`
维护者标签:
`<maintainer email="youremail@example.com" name="Your Name"></maintainer>`
许可证标签:
`<license>Apache 2.0</license>`
依赖项标签:
`<depend>std_msgs</depend>`
`<depend>rospy</depend>`
`<depend>roscpp</depend>`
 `<buildtool_depend>catkin</buildtool_depend>`

`<build_depend>roscpp</build_depend>`
   `<build_depend>rospy</build_depend>`
     `<build_depend>std_msgs</build_depend>`
     `<exec_depend>roscpp</exec_depend>`
     `<exec_depend>rospy</exec_depend>`
     `<exec_depend>std_msgs</exec_depend>`

**自定义CMakeLists.txt**

```
cmake_minimum_required(VERSION 3.0.2)
project(your_package_name)
# 1. 查找依赖的ROS包
find_package(catkin REQUIRED COMPONENTS
   roscpp      # C++支持
  rospy       # Python支持  
  std_msgs    # 标准消息
  geometry_msgs  # 几何消息
  sensor_msgs    # 传感器消息
  nav_msgs       # 导航消息
  tf2            # 坐标变换
  tf2_ros
  message_generation  # 消息生成
)

# 2. 启用Python支持
catkin_python_setup()

# 3. 定义消息/服务/动作
add_message_files(
  FILES
  RobotStatus.msg
  SensorData.msg
  # 可以添加多个.msg文件
)
add_service_files(
  FILES
  ControlRobot.srv
  # 可以添加多个.srv文件
)
add_action_files(
  FILES
  NavigateToGoal.action
)

# 4. 生成消息
generate_messages(
  DEPENDENCIES
  std_msgs
  geometry_msgs
  # 列出你的消息所依赖的消息包
)

# 5. 定义包配置
catkin_package(
  INCLUDE_DIRS include              # 导出头文件目录
  LIBRARIES my_robot_lib           # 导出的库
  CATKIN_DEPENDS roscpp rospy std_msgs message_runtime  # 运行时依赖
  DEPENDS system_lib               # 系统依赖
)

# 6. 指定头文件目录
# 包含自定义头文件目录
include_directories(
  include                    # 你的头文件目录
  ${catkin_INCLUDE_DIRS}     # ROS依赖的头文件
)

# 7. 创建可执行文件

# 创建可执行文件
add_executable(talker_node src/talker.cpp)
# 链接库
target_link_libraries(talker_node ${catkin_LIBRARIES})
# 添加依赖（确保消息先生成）
add_dependencies(talker_node ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
----------------------------------------------------------
# 节点1：发布者
add_executable(talker src/talker.cpp)
target_link_libraries(talker ${catkin_LIBRARIES})
add_dependencies(talker ${${PROJECT_NAME}_EXPORTED_TARGETS})

# 节点2：订阅者
add_executable(listener src/listener.cpp)
target_link_libraries(listener ${catkin_LIBRARIES})
add_dependencies(listener ${${PROJECT_NAME}_EXPORTED_TARGETS})

# 8. 安装规则

install(TARGETS
  talker
  listener
  my_library
  ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}    # 静态库
  LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}    # 动态库
  RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}    # 可执行文件
)
#  安装Python脚本
install(PROGRAMS
  scripts/my_python_script.py
  DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)

# 安装launch文件
install(DIRECTORY launch/
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}/launch
)
# 安装配置文件
install(DIRECTORY config/
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}/config
)
# 安装URDF/模型文件
install(DIRECTORY urdf/
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}/urdf
)
```

**构建软件包**
 `source /opt/ros/<distro>/setup.bash`

```
cd ~/catkin_ws/
ls src
catkin_make
```

**理解ROS节点**
节点是ROS系统中的基本执行单位，每个节点都是一个独立的进程，负责执行特定的任务。节点可以发布或订阅话题，调用服务，或者提供服务。节点之间通过话题、服务和参数进行通信。
roscore          # 启动总管家
rosnode list     # 查看所有运行的节点
rosnode info [节点名] # 查看节点详细信息

rosrun [package_name] [node_name]
e.g.rosrun turtlesim turtlesim_node
```
roscore = ros+core：主节点（为ROS提供命名服务) + rosout (stdout/stderr) + 参数服务器（会在以后介绍）
rosnode = ros+node：获取节点信息的ROS工具
rosrun = ros+run：运行给定的软件包中的节点
```
**话题**
话题：节点间传递信息的“广播频道”
发布者：在话题上广播消息的节点
订阅者：收听话题消息的节点

rostopic list      # 查看所有话题
rostopic echo [话题名] # 监听某个话题的消息
rostopic pub [话题名] [消息类型] [数据] # 手动发布消息
```
rostopic pub /turtle1/cmd_vel geometry_msgs/Twist "linear:
  x: 1.0
  y: 0.0
  z: 0.0
angular:
  x: 0.0
  y: 0.0
  z: 0.0"
  
//让小乌龟持续运动
  rostopic pub -r 10 /turtle1/cmd_vel geometry_msgs/Twist "linear: x: 1.0 y: 0.0 z: 0.0 angular: x: 0.0 y: 0.0 z: 0.0"
```


**创建ROS消息和服务**
msg：定义话题通信的数据结构
srv：定义服务通信的请求和响应格式

创建 .msg 或 .srv 文件
修改 package.xml 和 CMakeLists.txt
编译生成代码

+ 创建msg
`roscd beginner_tutorials`
`mkdir msg`
`echo "int64 num" > msg/Num.msg`

+ 创建srv
`roscd beginner_tutorials`
`mkdir srv`
`echo "int64 num" > srv/Num.srv`

+ rosservice
rosservice call /clear
rosservice type /spawn | rossrv show
rosservice call /spawn 2 2 0.2 ""// 调用/spawn服务，在(2,2)位置 spawn 一个新的乌龟，初始角度为0.2
+ rosparam
rosparam set /turtlesim/background_r 150
rosservice call /clear  修改背景

+ roslaunch启动多个节点
roslaunch [package_name] [launch_file_name.launch]
e.g.roslaunch turtlesim turtlesim.launch

launch文件
```
<launch>
同时启动两个turtlesim模拟器，而不会产生命名冲突。
  <group ns="turtlesim1">
    <node pkg="turtlesim" name="sim" type="turtlesim_node"/>
  </group>

  <group ns="turtlesim2">
    <node pkg="turtlesim" name="sim" type="turtlesim_node"/>
  </group>

  <node pkg="turtlesim" name="mimic" type="mimic">
    <remap from="input" to="turtlesim1/turtle1"/>
    <remap from="output" to="turtlesim2/turtle1"/>
  </node>

</launch>
```
运行launch文件
`roslaunch beginner_tutorials turtlemimic.launch`
新的终端发布：
`rostopic pub /turtlesim1/turtle1/cmd_vel geometry_msgs/Twist -r 1 -- '[2.0, 0.0, 0.0]' '[0.0, 0.0, -1.8]'`

**编写简单的发布者和订阅者**
发布者：不断发送消息的节点
订阅者：接收并处理消息的节点

编写发布者；
创建 `~/catkin_ws/src/my_communication/scripts/publisher.py`

```
import rospy
from std_msgs.msg import String

def simple_publisher():
    # 初始化节点，节点名必须唯一
    rospy.init_node('simple_publisher_node', anonymous=True)
    
    # 创建发布者，发布到 "chat_room" 话题，消息类型为 String，队列大小10
    pub = rospy.Publisher('chat_room', String, queue_size=10)
    
    # 设置发布频率：1Hz = 每秒1次
    rate = rospy.Rate(1)
    
    count = 0
    rospy.loginfo("发布者启动！开始发布消息...")
    
    while not rospy.is_shutdown():
        # 准备要发布的消息
        message_content = "学号+姓名！时间: {}".format( rospy.get_time())
        msg = String()
        msg.data = message_content
        
        # 发布消息
        pub.publish(msg)
        rospy.loginfo("已发布: %s", message_content)
        
        # 计数增加，等待下一个周期
        count += 1
        rate.sleep()

if __name__ == '__main__':
    try:
        simple_publisher()
    except rospy.ROSInterruptException:
        rospy.loginfo("发布者节点已关闭！")
```
编写订阅者：
创建 `~/catkin_ws/src/my_communication/scripts/subscriber.py`
```
import rospy
from std_msgs.msg import String
def simple_subscriber():
    # 初始化节点，节点名必须唯一
    rospy.init_node('simple_subscriber_node', anonymous=True)
    
    # 创建订阅者，订阅 "chat_room" 话题，消息类型为 String，回调函数为 message_callback
    sub = rospy.Subscriber('chat_room', String, message_callback)
    
    rospy.loginfo("订阅者启动！等待发布者发布消息...")
    
    # 保持节点运行，等待消息到达
    rospy.spin()
     # 回调函数：处理接收到的消息
def message_callback(msg):
    rospy.loginfo("订阅者收到消息: %s", msg.data)


```


**编写简单的服务和客户端**
服务端：提供服务的节点
客户端：请求服务的节点

+ 创建自定义服务定义
在 `~/catkin_ws/src/my_communication/srv/` 目录下创建 `AddTwoInts.srv` 文件
```
int64 a
int64 b
---
int64 sum
string message
```
修改 package.xml
添加依赖：
```
<build_depend>message_generation</build_depend>
<exec_depend>message_runtime</exec_depend>
```
修改 CMakeLists.txt
添加以下内容：
```
find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  std_msgs
  message_generation
)

add_service_files(
  FILES
  AddTwoInts.srv
)

generate_messages(
  DEPENDENCIES
  std_msgs
)

catkin_package(
  CATKIN_DEPENDS message_runtime roscpp rospy std_msgs
)
```

编译生成
`cd ~/catkin_ws`
`catkin_make`
`source devel/setup.bash`


+ 编写服务端
创建 `~/catkin_ws/src/my_communication/scripts/add_two_ints_server.py`
```
import rospy
from my_communication.srv import AddTwoInts, AddTwoIntsResponse

def handle_add_two_ints(req):
    rospy.loginfo("收到请求: %d + %d", req.a, req.b)
    # 计算结果
    sum_result = req.a + req.b
    message = "服务端计算结果: {} + {} = {}".format(req.a, req.b, sum_result)
    # 返回响应
    return AddTwoIntsResponse(sum_result, message)

def add_two_ints_server():
    # 初始化节点
    rospy.init_node('add_two_ints_server')
    # 创建服务
    s = rospy.Service('add_two_ints', AddTwoInts, handle_add_two_ints)
    rospy.loginfo("服务端已启动，等待客户端请求...")
    # 保持服务运行
    rospy.spin()

if __name__ == '__main__':
    try:
        add_two_ints_server()
    except rospy.ROSInterruptException:
        rospy.loginfo("服务端节点已关闭！")

```
+ 编写客户端
创建 `~/catkin_ws/src/my_communication/scripts/add_two_ints_client.py`
```
import rospy
from my_communication.srv import AddTwoInts, AddTwoIntsRequest

def add_two_ints_client(a, b):
    # 初始化节点
    rospy.init_node('add_two_ints_client')
    # 等待服务端启动
    rospy.wait_for_service('add_two_ints')
    try:
        # 创建服务代理
        add_two_ints = rospy.ServiceProxy('add_two_ints', AddTwoInts)
        # 创建请求
        req = AddTwoIntsRequest()
        req.a = a
        req.b = b
        # 调用服务
        resp = add_two_ints(req)
        # 打印结果
        rospy.loginfo("客户端请求: %d + %d", a, b)
        rospy.loginfo("服务端响应: %d, %s", resp.sum, resp.message)
    except rospy.ServiceException as e:
        rospy.logerr("服务调用失败: %s", e)
```

**测试发布者/订阅者**
终端1 - 
roscore

终端2 - 发布者：
cd ~/catkin_ws
source devel/setup.bash
rosrun my_communication publisher.py

终端3 - 订阅者：
cd ~/catkin_ws
source devel/setup.bash
rosrun my_communication subscriber.py

**测试服务/客户端**
终端4 - 服务端：
cd ~/catkin_ws
source devel/setup.bash
rosrun my_communication service_server.py

终端5 - 客户端：
cd ~/catkin_ws
source devel/setup.bash
//使用默认值
rosrun my_communication service_client.py
//或指定参数
rosrun my_communication service_client.py 10 20


**录制和回放数据**
rosbag record [话题名]   # 录制数据
rosbag play [文件名].bag # 回放数据

 **roswtf 检查**
 检查是否正确设置了环境变量，如 ROS_MASTER_URI、ROS_HOSTNAME 等。
 检查是否安装了必要的 ROS 包，如 roscpp、rospy、std_msgs 等
 节点之间的连接
 话题、服务、参数的通信状态
 Master 连接状态
 包依赖关系
 消息/服务定义
 文件权限问题
 
