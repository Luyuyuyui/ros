#include <geometry_msgs/msg/transform_stamped.hpp>

#include <rclcpp/rclcpp.hpp>
#include <tf2/LinearMath/Quaternion.h>
#include <tf2_ros/transform_broadcaster.h>
#include <turtlesim/msg/pose.hpp>

using std::placeholders::_1;

// 3.定义节点类；
class TurtleFrameBroadcaster : public rclcpp::Node
{
public:
  TurtleFrameBroadcaster(): Node("turtle_frame_broadcaster")
  {
    // 3-1.声明并解析乌龟名称参数；
    this->declare_parameter("turtle_name","turtle1");
    turtle_name = this->get_parameter("turtle_name").as_string();
    // 3-2.创建动态坐标变换发布方；
    tf_broadcaster_ = std::make_unique<tf2_ros::TransformBroadcaster>(*this);
    std::string topic_name = turtle_name + "/pose";

    // 3-3.创建乌龟位姿订阅方；
    subscription_ = this->create_subscription<turtlesim::msg::Pose>(
      topic_name, 10,
      std::bind(&TurtleFrameBroadcaster::handle_turtle_pose, this, _1));
  }

private:
  // 3-4.根据订阅到的乌龟位姿生成坐标帧并广播。   
  void handle_turtle_pose(const turtlesim::msg::Pose & msg)
  {
    // 组织消息
    geometry_msgs::msg::TransformStamped t;
    rclcpp::Time now = this->get_clock()->now();

    t.header.stamp = now;
    t.header.frame_id = "world";
    t.child_frame_id = turtle_name;

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
  std::string turtle_name;
};

int main(int argc, char * argv[])
{
  // 2.初始化 ROS 客户端；
  rclcpp::init(argc, argv);
  // 4.调用 spin 函数，并传入对象指针；
  rclcpp::spin(std::make_shared<TurtleFrameBroadcaster>());
  // 5.释放资源。
  rclcpp::shutdown();
  return 0;
}
