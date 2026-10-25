ros1切换ros2:
`sudo gedit .bashrc`


## 知识部分
**配置环境**

`source /opt/ros/humble/setup.bash`  # 每次新开终端都要执行🔄 
`echo "source /opt/ros/humble/setup.bash" >> ~/.bashrc` # 永久生效🔄 

验证方法：`ros2 --version`


**节点相关的CLI：**
ros2 run <package_name><executable_name>
ros2 node list
ros2 node info<node_name>



 **工作空间与包管理**
 ```
 ros2_ws/
├── src/          # 源代码
├── build/        # 编译文件
├── install/      # 安装文件
└── log/          # 日志文件
```
//创建流程
`mkdir -p ~/ros2_ws/src`
`cd ~/ros2_ws`# 进入工作空间
`colcon build`      # 编译
`source install/setup.bash`  # 激活工作空间

**包:基本功能单元**
获取功能包：
安装获取：sudo apt install ros-<version>-package_name
手动编译获取


`ros2 pkg create <package_name>`

创建一个包
`ros2 pkg create <package_name> --build-type ament_python --dependencies rclpy(依赖名字)`
(cmake,ament_cmake,ament_python) 
在 `src` 文件夹里生成了一个名为 `my_first_package` 的新文件夹，里面包含了包的基本结构
ros2 pkg executables <package_name>列出
ros2 pkg list

**colcon**
功能包构建工具--编译
colcon build   //对所有功能包编译
colcon build --packages-select 包名字  //只编译一个包
colcon build --symlink-install 调整python时，不必重新build了


**节点 :ROS2 中的可执行程序**



🔧 🔧 🔧 
**编写发布者/订阅者**
发布者 (Publisher)：发送消息的节点
订阅者 (Subscriber)：接收消息的节点
导入接口
创建节点类
实现发布/订阅逻辑
编译运行
`colcon build --packages-select <package_name>`
`source install/setup.bash`
`ros2 run <package_name> <node_name>`


运行节点：
进入工作空间
source install/setup.bash

自定义节点：
创作工作空间——创建功能包

mkdir -p ros2_ws/src
code./
ros2 pkg create mypackage --build-type ament_python --dependencies rclpy
在__init__.py同级目录下创建test.py

ros2 pkg create mypackage --build-type ament_cmake --dependencies rclcpp
在src下创建test.cpp



导入库文件
初始化客户端库
新建节点
spin循环节点
关闭客户端库

==python==
非OOP：
```
import rclpy
from rclpy.node import Node

def main(args=None):
    rclpy.init(args=args)
    node=Node("mynode")
    node.get_logger().info("hello world")
    rclpy.spin(node) //循环节点
    rclpy.shundown()
```
在setup.py  entry_point下加入"node=mypackage.test:main"


OOP:
```
import rclpy
from rclpy.node import Node

class WriteNode(Node):
    def __init__(self.name):
        super().__init__('hello')
        self.get_logger().info("hello world")
        
def main(args=None):
    rclpy.init(args=args)
    node=WriteNode("mynode")
    rclpy.spin(node) //循环节点
    rclpy.shundown()

```
==c++==
非OOP
输入：
```
#include"rclcpp/rclcpp.hpp"

int main(int argc,char**argv)
{
    rclcpp::init(argc,argv);
    auto node=std::make_shared<rclcpp::Node>("mynode");
    RCLCPP_INFO(node->get_logger(),"hello world");
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}
```
修改CmakeLists
```
add_executable(test src/test.cpp)
ament_target_dependencies(test rclcpp)

install(TARGETS
    test
    DESTINATION lib/$(PROJECT_NAME)   //放到install目录下
)
```
OOP:
```
#include"rclcpp/rclcpp.hpp"
#include"std_msgs/msg/string.hpp"

using std::placeholders::_1;
using std::placeholders::_2;

class WriteNode:public rclcpp::Node
{
public:
    WriteNode(std::string name):Node(name)
    {
        RCLCPP_INFO(node->get_logger(),"hello world%s",name.c_str());
        sub_msg=this->create_subscription<std_msg::msg::String>("hello",10,std::bind(&WriteNode::topic_callback,this,_1));
    }
private:
    rclcpp::Subscription<std_msg::msg::String>::SharePtr sub_msg;
}

void topic_callback(const std_msgs::msg::String::SharePtr msg)
{
    RCLCPP_INFO(node->get_logger(),"收到%s",mas->data.c_str());
}
int main(int argc,char**argv)
{
    rclcpp::init(argc,argv);
    auto node=std::make_shared<WriteNode>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}


```

+ 编写发布者节点 🔧 

  编辑 `my_first_publisher.py` 文件：
  ```
  import rclpy
  from rclpy.node import Node
  from std_msgs.msg import String   #导入消息类型

  class MyFirstPublisher(Node):
    def __init__(self):
        super().__init__('my_first_publisher')
        self.publisher_ = self.create_publisher(String, 'chatter', 10)    #创建发布者 类型 名字 
        timer_period = 1.0         # seconds
        self.timer = self.create_timer(timer_period, self.timer_callback)
        self.i = 0   #编号

    def timer_callback(self):
        msg = String()
        msg.data = 'Hello, World! %d' % self.i   # 发布消息
        self.publisher_.publish(msg)
        self.get_logger().info('Publishing: "%s"' % msg.data)
        self.i += 1

+ 编写订阅者节点 🔧 
  编辑 `my_first_subscriber.py` 文件：
  ```
  import rclpy
  from rclpy.node import Node
  from std_msgs.msg import String,UInt32 #导入订阅话题接口类型

  class MyFirstSubscriber(Node):
    def __init__(self):
        super().__init__('my_first_subscriber')
        self.subscription = self.create_subscription(
            String,
            'chatter',
            self.listener_callback,
            10)
        self.subscription    # 防止未使用变量警告
    def listener_callback(self, msg):  
        self.get_logger().info('I heard: "%s"' % msg.data)


+ 编译并运行 🔧 
  cd ~/ros2_ws
 编译
colcon build
 **激活工作空间（重要！）**
source install/setup.bash
 运行发布者（开一个终端）
ros2 run my_first_package publisher
 运行订阅者（另一个终端，同样要先source）
ros2 run my_first_package subscriber



**主/话题 :用于节点之间通信的消息传递机制**
`ros2 topic list`              # 查看所有主题
`ros2 topic echo/<topic_name>`
`ros2 topic echo <topic_name>` # 监听主题消息
`ros2 topic info <topic_name>` # 查看主题信息
rqt_graph

话题发布者：
导入消息类型——声明创建发布者——编写发布逻辑和数据

**服务 :节点之间的远程过程调用机制**（双向，消息是单向）
**参数 :节点的配置选项**
**动作 :用于处理复杂任务的机制，包括状态机、进度报告等。**


**turtlesim**: 一个简单的机器人模拟器，用于演示 ROS2 中的基本概念。
`ros2 run turtlesim turtlesim_node`

**rqt**: 一个可视化工具，用于调试和可视化 ROS2 中的节点、主题、服务等。
`rqt_graph`

**数据录制与回放**
`ros2 bag record ` # 录制所有主题
`ros2 bag play <bag_file>` # 回放 bag 文件


**消息接口**
话题服务动作
消息接口定义了节点之间通信的消息格式。
消息/话题 (.msg)：主题通信的数据结构  int64 num
服务 (.srv)：服务通信的请求/响应结构  int64 a  int64 b --- int64 num
动作(.action)

ros2 interface list 查看所有接口
ros2 interface show 路径


`自定义接口步骤`
创建 .msg 或 .srv 文件
修改 package.xml 和 CMakeLists.txt
编译生成代码

🔧 🔧 🔧 
创建一个新的功能包xxx_interfaces
例子：**以创建一个 Student.msg 为例**
进入包目录，创建 msg 文件夹，创建 Student.msg 文件
`gedit msg/Student.msg`
 内容如下
string name
uint32 age
string major
sensor_msgs/Image image  #调用已有的类型

修改 package.xml
```
<!-- 在 <buildtool_depend> 之后添加 -->

<depend>sensor_msgs</depend>
<build_depend>rosidl_default_generators</build_depend>
<exec_depend>rosidl_default_runtime</exec_depend>
<member_of_group>rosidl_interface_packages</member_of_group>
```
修改 CMakeLists.txt

```
<!--在 find_package(ament_cmake_python REQUIRED) 附近添加-->
find_package(sensor_msgs REQUIRED)
find_package(rosidl_default_generators REQUIRED)

rosidl_generate_interfaces(${PROJECT_NAME}
  "msg/Student.msg"
  DEPENDENCES sensor_msgs
)  //声明消息文件

ament_export_dependencies(rosidl_default_runtime)//添加依赖关系
```

返回工作空间并编译
cd ~/ros2_ws
colcon build --packages-select my_package
激活工作空间


在代码中使用自定义接口:
发布者：from my_package.msg import Student  # 导入自定义消息
订阅者：from my_package.msg import Student  # 导入自定义消息


**创建自定义服务接口**
xxx_interfaces目录下
新建srv 文件夹  新建xx.srv
```
string name
uint32 money
---
bool success
uint32 money
```
修改 CMakeLists.txt
rosidl_generate_interfaces(${PROJECT_NAME}
  "srv/xx.srv"
  DEPENDENCES sensor_msgs
)  //声明消息文件

添加依赖；修改package.xml
<depend>xx.interfaces</depend>
find_package()
在ament_target_dependence加入

编译
**实现服务通信**
==python==
在publisher代码中添加
服务端：
```
from my_package.srv import AddTwoInts
在__init__中声明传参；
在类中添加：#创建服务端
def handle_add_two_ints(self, request, response):
    response.sum = request.a + request.b
    return response
```
客户端：在subscriber中
```
from my_package.srv import AddTwoInts

client = self.create_client(AddTwoInts, 'add_two_ints')
request = AddTwoInts.Request()
request.a = 5
request.b = 3

#异步请求
#self.borrow_cilent.call_async(requst).add_done_callback(self.borrow_response_callback)
```

==cpp==
服务端
```
#include"xx_interfaces"
void self_callback(
    const xx_interfaces::srv::selfnovel::Request::SharePtr request,
     const xx_interfaces::srv::selfnovel::Request::SharePtr response
)
```

多线程：
在类中声明回调组
rclcpp::CallbackGroup::SharePtr callback_group;
在main中：
rclcpp::executors::MultiThreadExecutor executor;
executor.add_node(node);
将rclcpp替换成
executor.spin();


**使用 rosdep 管理依赖**
`rosdep install --from-paths src --ignore-src -r -y`
自动安装包依赖的工具,确保代码在不同机器上都能正确运行

package.xml：声明依赖
CMakeLists.txt：查找依赖


**参数**
声明并设置
在__init__中：
self.declare_parameter("time_period",5)
在回调函数中：
#回调更新回调期
time_period=self.get_parameter('time_period').get_parameter_value().integer_value
#更新回调周期
self.timer.timer_period_ns=timer_period*(1000*1000*1000)


键盘控制节点
ros2 eun turtlesim turtle_teleop_key
**创建自定义动作** 
 Goal 目标
int32 order
 Result 结果  
int32[] sequence
 Feedback 反馈
int32[] partial_sequence


流程：
创建 action/YourAction.action 文件
例如：Fibonacci.action
int32 order
int32[] sequence
int32[] partial_sequence

在 CMakeLists.txt中：
```
find_package(rosidl_default_generators REQUIRED)

rosidl_generate_interfaces(${PROJECT_NAME}
  =="action/Fibonacci.action"==
)
```
在 package.xml 中:
```
<buildtool_depend>rosidl_default_generators</buildtool_depend>

<depend>action_msgs</depend>

<member_of_group>rosidl_interface_packages</member_of_group>
```

构建包含 Fibonacci 动作定义的包:
```
cd ~/ros2_ws
colcon build
```

检查动作是否构建成功:
`. install/setup.bash`

`ros2 interface show action_tutorials_interfaces/action/Fibonacci`

🔧 🔧 🔧 
**动作服务器**
接上例：
fibonacci_action_server.py

```
import time

import rclpy
from rclpy.action import ActionServer
from rclpy.node import Node

from action_tutorials_interfaces.action import Fibonacci


class FibonacciActionServer(Node):

    def __init__(self):
        super().__init__('fibonacci_action_server')
        self._action_server = ActionServer(
            self,
            Fibonacci,
            'fibonacci',
            self.execute_callback)

    def execute_callback(self, goal_handle):
        self.get_logger().info('Executing goal...')

        feedback_msg = Fibonacci.Feedback()
        feedback_msg.partial_sequence = [0, 1]

        for i in range(1, goal_handle.request.order):
            feedback_msg.partial_sequence.append(
                feedback_msg.partial_sequence[i] + feedback_msg.partial_sequence[i-1])
            self.get_logger().info('Feedback: {0}'.format(feedback_msg.partial_sequence))
            goal_handle.publish_feedback(feedback_msg)
            time.sleep(1)

        goal_handle.succeed()

        result = Fibonacci.Result()
        result.sequence = feedback_msg.partial_sequence
        return result


def main(args=None):
    rclpy.init(args=args)

    fibonacci_action_server = FibonacciActionServer()

    rclpy.spin(fibonacci_action_server)


if __name__ == '__main__':
    main()
```


//运行服务器：
`python3 fibonacci_action_server.py`
`ros2 action send_goal fibonacci action_tutorials_interfaces/action/Fibonacci "{order: 5}"`
确认已经发布反馈：
`ros2 action send_goal --feedback fibonacci action_tutorials_interfaces/action/Fibonacci "{order: 5}"`

🔧 🔧 🔧 
**动作客户端**
fibonacci_action_client.py

```
import rclpy
from rclpy.action import ActionClient
from rclpy.node import Node

from action_tutorials_interfaces.action import Fibonacci


class FibonacciActionClient(Node):

    def __init__(self):
        super().__init__('fibonacci_action_client')
        self._action_client = ActionClient(self, Fibonacci, 'fibonacci')
    //发送目标
    def send_goal(self, order):
        goal_msg = Fibonacci.Goal()
        goal_msg.order = order

        self._action_client.wait_for_server()

        self._send_goal_future = self._action_client.send_goal_async(goal_msg, feedback_callback=self.feedback_callback)

        self._send_goal_future.add_done_callback(self.goal_response_callback)
    //处理目标响应
    def goal_response_callback(self, future):
        goal_handle = future.result()
        if not goal_handle.accepted:
            self.get_logger().info('Goal rejected :(')
            return

        self.get_logger().info('Goal accepted :)')

        self._get_result_future = goal_handle.get_result_async()
        self._get_result_future.add_done_callback(self.get_result_callback)
    //处理结果响应
    def get_result_callback(self, future):
        result = future.result().result
        self.get_logger().info('Result: {0}'.format(result.sequence))
        rclpy.shutdown()
    //反馈消息的回调函数
    def feedback_callback(self, feedback_msg):
        feedback = feedback_msg.feedback
        self.get_logger().info('Received feedback: {0}'.format(feedback.partial_sequence))


def main(args=None):
    rclpy.init(args=args)

    action_client = FibonacciActionClient()

    action_client.send_goal(10)

    rclpy.spin(action_client)


if __name__ == '__main__':
    main()
```
🔧 🔧 🔧 
**launch**
需要启动的节点太多；节点的启动有依赖关系；可以同事启动和配置多个包含ros2节点的可执行文件 (python xml yaml)

使用 turtlesim 包及其可执行文件来创建一个 ROS 2 启动文件
launch/turtlesim_mimic_launch.py:为两个乌龟模拟窗口启动，并让一个乌龟模仿另一个乌龟的运动

以下是python 格式的
```
//引入了一些 Python launch 模块
from launch import LaunchDescription   对文件内容描述
from launch_ros.actions import Node

//启动描述
def generate_launch_description():
   return LaunchDescription([

   ])

//启动了两个乌龟模拟窗口
Node(
    package='turtlesim',
    namespace='turtlesim1',
    executable='turtlesim_node',
    name='sim'
),
Node(
    package='turtlesim',
    namespace='turtlesim2',
    executable='turtlesim_node',
    name='sim'
),
//使用重映射启动 mimic 节点
Node(
    package='turtlesim',
    executable='mimic',
    name='mimic',
    remappings=[
      ('/input/pose', '/turtlesim1/turtle1/pose'),
      ('/output/cmd_vel', '/turtlesim2/turtle1/cmd_vel'),
    ]
)
```
修改setup.py文件
```
from glob import glob
import os

在data_files中添加
(os.path.join('share',package_name,'launch'),glob('launch/*.launch.py))
```
启动运行
ros2 launch turtlesim turtlesim_mimic_launch.py

**rosbag2**
记录话题数据，进行重放
ros2 bag rocord /话题名字
ros2 bag record -a 记录所有话题
**RQT**
GUI框架，各种框架


**坐标变换**
`所有的坐标关系是通过一颗树形结构组成,`
两个坐标系间的tf消息组成
```
    header: 
      seq: 0
      stamp: 
        secs: 1621956538
        nsecs: 108681917
      frame_id: "parent"
    child_frame_id: "child"
    transform: 
      translation: 
        x: -0.45
        y: 0.2
        z: 0.0
      rotation: 
        x: 0.0
        y: 0.0
        z: 0.0
        w: 1.0
```

位姿：位置+姿态
用旋转矩阵描述
参考坐标系
坐标系A中一个点P的位置，以P为原点再建立坐标系，描述新和旧之间姿态的关系来表示P的姿态


平移变换：姿态都相同
旋转坐标变换：

TF变换如何实现：
1、广播TF变换  发布两个坐标系之间的位置关系
2、监听TF变换  获取两个坐标系之间的位置关系

`br.sendTransform()`; 该函数可以发送我们的tf变换的消息
`tf::StampedTransform(transform, ros::Time::now(), child_frame_id, frame_id); `该函数可以创建一条我们需要发送的tf消息。他会自动更根据ros::Time::now()的时间，为我们添加stamp这样的时间戳。
`transform`是一个类，它包含了`translation、 rotation`这两个tf需要用到的元素。
`transform.setOrigin( tf::Vector3(0.0, 2.0, 0.0) ) `可以修改transform.translation的值，参数为相对于父坐标系的x,y,z
`transform.setRotation( tf::Quaternion(0, 0, 0, 1) )`可以修改transform.rotation的值，参数为2X2的旋转矩阵。
`tf::Quaternion q`; `q.setRPY(0, 0, msg->theta)`; `transform.setRotation(q)`;但是手动计算旋转矩阵太麻烦了，所以我们可以通过创建`Quaternion`对象，并通过`rpy`作为参数，实例化该对象，它会自动根据rpy的三个参数创建旋转矩阵。



`listener.lookupTransform(child_frame_id, frame_id, ros::Time(0), transform);`
`ros::Time(0)`代表 （tf树由后台计算处理） 最新可用的tf树的时间



两个接口：
`geometry_msgs/msg/TransformStamped`
`geometry_msgs/msg/PointStamped`

坐标变换广播：静态坐标系相对关系与动态坐标系相对关系

编写广播实现；
编辑配置文件；
编译；
执行；
在 rviz2 中查看坐标系关系

静态广播器：C++
```
#include <geometry_msgs/msg/transform_stamped.hpp>
#include <rclcpp/rclcpp.hpp>
#include <tf2/LinearMath/Quaternion.h>
#include <tf2_ros/static_transform_broadcaster.h>

using std::placeholders::_1;

// 4.定义节点类；
class MinimalStaticFrameBroadcaster : public rclcpp::Node
{
public:
  explicit MinimalStaticFrameBroadcaster(char * transformation[]): Node("minimal_static_frame_broadcaster")
  {
    // 4-1.创建静态坐标变换发布方；
    tf_publisher_ = std::make_shared<tf2_ros::StaticTransformBroadcaster>(this);

    this->make_transforms(transformation);
  }

private:
  // 4-2.组织并发布消息。
  void make_transforms(char * transformation[])
  {
    // 组织消息
    geometry_msgs::msg::TransformStamped t;

    rclcpp::Time now = this->get_clock()->now();
    t.header.stamp = now;
    t.header.frame_id = transformation[7];
    t.child_frame_id = transformation[8];

    t.transform.translation.x = atof(transformation[1]);
    t.transform.translation.y = atof(transformation[2]);
    t.transform.translation.z = atof(transformation[3]);
    tf2::Quaternion q;
    q.setRPY(
      atof(transformation[4]),
      atof(transformation[5]),
      atof(transformation[6]));
    t.transform.rotation.x = q.x();
    t.transform.rotation.y = q.y();
    t.transform.rotation.z = q.z();
    t.transform.rotation.w = q.w();

    // 发布消息
    tf_publisher_->sendTransform(t);
  }
  std::shared_ptr<tf2_ros::StaticTransformBroadcaster> tf_publisher_;
};

int main(int argc, char * argv[])
{
  // 2.判断终端传入的参数是否合法；
  auto logger = rclcpp::get_logger("logger");

  if (argc != 9) {
    RCLCPP_INFO(
      logger, "运行程序时请按照：x y z roll pitch yaw frame_id child_frame_id 的格式传入参数");
    return 1;
  }

  // 3.初始化 ROS 客户端；
  rclcpp::init(argc, argv);
  // 5.调用 spin 函数，并传入对象指针；
  rclcpp::spin(std::make_shared<MinimalStaticFrameBroadcaster>(argv));
  // 6.释放资源。
  rclcpp::shutdown();
  return 0;
}
```
CmakeList.txt:
```
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(tf2 REQUIRED)
find_package(tf2_ros REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(turtlesim REQUIRED)

add_executable(demo01_static_tf_broadcaster src/demo01_static_tf_broadcaster.cpp)
ament_target_dependencies(
  demo01_static_tf_broadcaster
  "rclcpp"
  "tf2"
  "tf2_ros"
  "geometry_msgs"
  "turtlesim"
)

install(TARGETS demo01_static_tf_broadcaster
  DESTINATION lib/${PROJECT_NAME})

```

**动态广播器**


步骤：
    1.包含头文件；
    2.初始化 ROS 客户端；
    3.定义节点类；
      3-1.创建动态坐标变换发布方；
      3-2.创建乌龟位姿订阅方；
      3-3.根据订阅到的乌龟位姿生成坐标帧并广播。
    4.调用 spin 函数，并传入对象指针；
    5.释放资源。

```
// 1.包含头文件；
#include <geometry_msgs/msg/transform_stamped.hpp>

#include <rclcpp/rclcpp.hpp>
#include <tf2/LinearMath/Quaternion.h>
#include <tf2_ros/transform_broadcaster.h>
#include <turtlesim/msg/pose.hpp>

using std::placeholders::_1;

// 3.定义节点类；
class MinimalDynamicFrameBroadcaster : public rclcpp::Node
{
public:
  MinimalDynamicFrameBroadcaster(): Node("minimal_dynamic_frame_broadcaster")
  {
    // 3-1.创建动态坐标变换发布方；
    tf_broadcaster_ = std::make_unique<tf2_ros::TransformBroadcaster>(*this);

    std::string topic_name = "/turtle1/pose";

    // 3-2.创建乌龟位姿订阅方；
    subscription_ = this->create_subscription<turtlesim::msg::Pose>(
      topic_name, 10,
      std::bind(&MinimalDynamicFrameBroadcaster::handle_turtle_pose, this, _1));
  }

private:
  // 3-3.根据订阅到的乌龟位姿生成坐标帧并广播。   
  void handle_turtle_pose(const turtlesim::msg::Pose & msg)
  {
    // 组织消息
    geometry_msgs::msg::TransformStamped t;
    rclcpp::Time now = this->get_clock()->now();

    t.header.stamp = now;
    t.header.frame_id = "world";
    t.child_frame_id = "turtle1";

    t.transform.translation.x = msg.x;
    t.transform.translation.y = msg.y;
    t.transform.translation.z = 0.0;

    tf2::Quaternion q;
    q.setRPY(0, 0, msg.theta);
    t.transform.rotation.x = q.x();
    t.transform.rotation.y = q.y();
    t.transform.rotation.z = q.z();
    t.transform.rotation.w = q.w();
    // 发布消息
    tf_broadcaster_->sendTransform(t);
  }
  rclcpp::Subscription<turtlesim::msg::Pose>::SharedPtr subscription_;
  std::unique_ptr<tf2_ros::TransformBroadcaster> tf_broadcaster_;
};

int main(int argc, char * argv[])
{
  // 2.初始化 ROS 客户端；
  rclcpp::init(argc, argv);
  // 4.调用 spin 函数，并传入对象指针；
  rclcpp::spin(std::make_shared<MinimalDynamicFrameBroadcaster>());
  // 5.释放资源。
  rclcpp::shutdown();
  return 0;
}
```
CmakeList.txt添加：
```
add_executable(demo02_dynamic_tf_broadcaster src/demo02_dynamic_tf_broadcaster.cpp)
ament_target_dependencies(
  demo02_dynamic_tf_broadcaster
  "rclcpp"
  "tf2"
  "tf2_ros"
  "geometry_msgs"
  "turtlesim"
)

install(TARGETS demo01_static_tf_broadcaster
  demo02_dynamic_tf_broadcaster
  DESTINATION lib/${PROJECT_NAME})
```

坐标点发布：
  1.包含头文件；
        2.初始化 ROS 客户端；
        3.定义节点类；
            3-1.创建坐标点发布方；
            3-2.创建定时器；
            3-3.组织并发布坐标点消息。
        4.调用 spin 函数，并传入对象指针；
        5.释放资源。

```
// 1.包含头文件；
#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/point_stamped.hpp"

using namespace std::chrono_literals;

// 3.定义节点类；
class MinimalPointPublisher: public rclcpp::Node {
public:
    MinimalPointPublisher(): Node("minimal_point_publisher"),x(0.1){
        // 3-1.创建坐标点发布方；
        point_pub_ = this->create_publisher<geometry_msgs::msg::PointStamped>("point",10);
        // 3-2.创建定时器；
        timer_ = this->create_wall_timer(0.1s,std::bind(&MinimalPointPublisher::on_timer, this));
    }
private:
    void on_timer(){
        // 3-3.组织并发布坐标点消息。
        geometry_msgs::msg::PointStamped point;
        point.header.frame_id = "laser";
        point.header.stamp = this->now();
        x += 0.004;
        point.point.x = x;
        point.point.y = 0.0;
        point.point.z = 0.1;        
        point_pub_->publish(point);
    }
    rclcpp::Publisher<geometry_msgs::msg::PointStamped>::SharedPtr point_pub_;
    rclcpp::TimerBase::SharedPtr timer_;
    double_t x;
};

int main(int argc, char const *argv[])
{
    // 2.初始化 ROS 客户端；
    rclcpp::init(argc,argv);
    // 4.调用 spin 函数，并传入对象指针；
    rclcpp::spin(std::make_shared<MinimalPointPublisher>());
    // 5.释放资源。
    rclcpp::shutdown();
    return 0;
}
```


```
add_executable(demo03_point_publisher src/demo03_point_publisher.cpp)
ament_target_dependencies(
  demo03_point_publisher
  "rclcpp"
  "tf2"
  "tf2_ros"
  "geometry_msgs"
  "turtlesim"
)

install(TARGETS demo01_static_tf_broadcaster
  demo02_dynamic_tf_broadcaster
  demo03_point_publisher
  DESTINATION lib/${PROJECT_NAME})
```

**坐标变换监听**

编写坐标变换程序实现；
编辑配置文件；
编译；
执行。

坐标系与坐标点变换：
  1.包含头文件；
    2.初始化 ROS 客户端；
    3.定义节点类；
      3-1.创建tf缓存对象指针；
      3-2.创建tf监听器；
      3-3.按照条件查找符合条件的坐标系并生成变换后的坐标帧。
    4.调用 spin 函数，并传入对象指针；
    5.释放资源。
```
#include "rclcpp/rclcpp.hpp"
#include "tf2_ros/transform_listener.h"
#include "tf2_ros/buffer.h"
#include "tf2/LinearMath/Quaternion.h"

using namespace std::chrono_literals;

// 3.定义节点类；
class MinimalFrameListener : public rclcpp::Node {
public:
  MinimalFrameListener():Node("minimal_frame_listener"){
    tf_buffer_ = std::make_unique<tf2_ros::Buffer>(this->get_clock());
    transform_listener_ = std::make_shared<tf2_ros::TransformListener>(*tf_buffer_);
    timer_ = this->create_wall_timer(1s, std::bind(&MinimalFrameListener::on_timer,this));
  }

private:
  void on_timer(){
    try
    {
      auto transformStamped = tf_buffer_->lookupTransform("camera","laser",tf2::TimePointZero);
      RCLCPP_INFO(this->get_logger(),"----------------------转换结果----------------------");
      RCLCPP_INFO(this->get_logger(),"frame_id:%s",transformStamped.header.frame_id.c_str());
      RCLCPP_INFO(this->get_logger(),"child_frame_id:%s",transformStamped.child_frame_id.c_str());
      RCLCPP_INFO(this->get_logger(),"坐标:(%.2f,%.2f,%.2f)",
                transformStamped.transform.translation.x,
                transformStamped.transform.translation.y,
                transformStamped.transform.translation.z);

    }
    catch(const tf2::LookupException& e)
    {
      RCLCPP_INFO(this->get_logger(),"坐标变换异常：%s",e.what());
    }


  }
  rclcpp::TimerBase::SharedPtr timer_;
  std::shared_ptr<tf2_ros::TransformListener> transform_listener_;
  std::unique_ptr<tf2_ros::Buffer> tf_buffer_;
};

int main(int argc, char const *argv[])
{
  // 2.初始化 ROS 客户端；
  rclcpp::init(argc,argv);
  // 4.调用 spin 函数，并传入对象指针；
  rclcpp::spin(std::make_shared<MinimalFrameListener>());
  // 5.释放资源。
  rclcpp::shutdown();
  return 0;
}
```

//乌龟跟随案例实现
将turtle1确定为目标点，控制turtle2移动 ；先分别广播 turtle1 相对于 world 和 turtle2 相对于 world 的坐标系关系，然后再通过监听坐标系关系进一步获取 turtle1 相对于 turtle2 的坐标系关系；

流程：
编写程序调用 /spawn 服务生成一只新乌龟；
编写坐标变换广播实现，通过该实现可以广播 turtle1 相对于 world 和 turtle2 相对于 world 的坐标系关系；
编写坐标变换监听实现，获取 turtle1 相对于 turtle2 的坐标系关系并生成控制 turtle2 运动的速度指令；
编写 launch 文件集成相关节点；
编辑配置文件；
编译；
执行；

==cpp==
+ 编写客户端，发送请求生成一只新的乌龟
```
TurtleSpawnClient():Node("turtle_spawn_client"){
      // 声明并获取参数；
      this->declare_parameter("x",2.0);
      this->declare_parameter("y",8.0);
      this->declare_parameter("theta",0.0);
      this->declare_parameter("turtle_name","turtle2");
      x = this->get_parameter("x").as_double();
      y = this->get_parameter("y").as_double();
      theta = this->get_parameter("theta").as_double();
      turtle_name = this->get_parameter("turtle_name").as_string();
      
      // 创建客户端；
      client = this->create_client<turtlesim::srv::Spawn>("/spawn");
    }


     // 组织请求数据并发送；
    rclcpp::Client<turtlesim::srv::Spawn>::FutureAndRequestId send_request(){
      auto request = std::make_shared<turtlesim::srv::Spawn::Request>();
      request->x = x;
      request->y = y;
      request->theta = theta;
      request->name = turtle_name;
      return client->async_send_request(request);
    }
```

发布乌龟坐标系到窗口坐标系的坐标变换

声明并解析乌龟名称参数；
创建动态坐标变换发布方；
创建乌龟位姿订阅方；
根据订阅到的乌龟位姿生成坐标帧并广播。
`#include <geometry_msgs/msg/transform_stamped.hpp>`
```
 // 声明并解析乌龟名称参数；
    this->declare_parameter("turtle_name","turtle1");
    turtle_name = this->get_parameter("turtle_name").as_string();
    // 创建动态坐标变换发布方；
    tf_broadcaster_ = std::make_unique<tf2_ros::TransformBroadcaster>(*this);
    std::string topic_name = turtle_name + "/pose";

    // 创建乌龟位姿订阅方；
    subscription_ = this->create_subscription<turtlesim::msg::Pose>(
      topic_name, 10,
      std::bind(&TurtleFrameBroadcaster::handle_turtle_pose, this, _1));

```
广播的坐标系消息，并生成 turtle2 相对于 turtle1 的坐标系数据，进一步生成控制 turtle2 运动的速度指令

创建tf缓存对象指针；
创建tf监听器；
按照条件查找符合条件的坐标系并生成变换后的坐标帧。
生成 turtle2 的速度指令，并发布。


编写 launch 文件







## ROS1和ROS2区别与联系
**区别**
**ROS1（集中式架构）：**
+ 依赖中心化的roscore，使用一个中心化的 roscore（包含 Master 节点）来管理整个系统。所有节点在启动时都必须向 Master 注册，并通过 Master 来查找和发现其他节点。

+ 主要支持 Linux

+ 使用的是 catkin 构建系统和工具来管理包

**ROS2（分布式去中心化架构）：**
+ 没有中心管理节点 ，节点启动后，会通过底层通信中间件 （DDS）自动向网络中广播自己的存在和信息。无需中心节点，直接运行节点

+ 主要支持 Linux（包括 Ubuntu），还支持 Windows 和 macOS。

+ 内建了安全功能，DDS 中的安全特性（如身份验证、加密、访问控制）被集成进了 ROS2，能够提供更强的通信安全保障。

**联系**
+ 都采用发布-订阅和客户端-服务端模式
+ 包管理系统，相同的包概念，依赖管理相似性
+ 都有丰富的工具和库，如 rqt、rviz 等，用于可视化调试和监控
+ 都支持多语言编程，Python、C++
+ CMake 为基础