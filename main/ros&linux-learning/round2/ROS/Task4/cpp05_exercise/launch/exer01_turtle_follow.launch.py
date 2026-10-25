from launch import LaunchDescription
from launch_ros.actions import Node
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration

def generate_launch_description():
    # 声明参数
    turtle1 = DeclareLaunchArgument(name="turtle1",default_value="turtle1")
    turtle2 = DeclareLaunchArgument(name="turtle2",default_value="turtle2")
    # 启动 turtlesim_node 节点
    turtlesim_node = Node(package="turtlesim", executable="turtlesim_node", name="t1")
    # 生成一只新乌龟
    spawn = Node(package="cpp05_exercise", executable="exer01_tf_spawn",
                name="spawn1",
                parameters=[{"turtle_name":LaunchConfiguration("turtle2")}]
    )
    # tf 广播
    tf_broadcaster1 = Node(package="cpp05_exercise",executable="exer02_tf_broadcaster",
                            name="tf_broadcaster1")
    tf_broadcaster2 = Node(package="cpp05_exercise",executable="exer02_tf_broadcaster",
                            name="tf_broadcaster1",
                            parameters=[{"turtle_name":LaunchConfiguration("turtle2")}])
    # tf 监听
    tf_listener = Node(package="cpp05_exercise",executable="exer03_tf_listener",
                            name="tf_listener",
                            parameters=[{"target_frame":LaunchConfiguration("turtle2"),"source_frame":LaunchConfiguration("turtle1")}]
                            )
    return LaunchDescription([turtle1,turtle2,turtlesim_node,spawn,tf_broadcaster1,tf_broadcaster2,tf_listener])
