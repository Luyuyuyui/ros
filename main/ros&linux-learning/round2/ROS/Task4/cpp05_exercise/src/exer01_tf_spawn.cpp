#include "rclcpp/rclcpp.hpp"
#include "turtlesim/srv/spawn.hpp"

using namespace std::chrono_literals;

// 定义节点类；
class TurtleSpawnClient: public rclcpp::Node{
  public:
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
    // 等待服务连接；
    bool connect_server(){
      while (!client->wait_for_service(1s))
      {
        if (!rclcpp::ok())
        {
          RCLCPP_INFO(this->get_logger(),"客户端退出！");
          return false;
        }

        RCLCPP_INFO(this->get_logger(),"服务连接中，请稍候...");
      }
      return true;
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

  private:
    rclcpp::Client<turtlesim::srv::Spawn>::SharedPtr client;
    float_t x,y,theta;
    std::string turtle_name;
};

int main(int argc, char ** argv)
{
  // 初始化 ROS2 客户端；
  rclcpp::init(argc,argv);

  // 创建对象指针并调用其功能；
  auto client = std::make_shared<TurtleSpawnClient>();
  bool flag = client->connect_server();
  if (!flag)
  {
    RCLCPP_INFO(client->get_logger(),"服务连接失败!");
    return 0;
  }

  auto response = client->send_request();

  // 处理响应
  if (rclcpp::spin_until_future_complete(client,response) == rclcpp::FutureReturnCode::SUCCESS)
  {
    RCLCPP_INFO(client->get_logger(),"请求正常处理");
    std::string name = response.get()->name;
    if (name.empty())
    {
        RCLCPP_INFO(client->get_logger(),"乌龟重名导致生成失败！");
    } else {
        RCLCPP_INFO(client->get_logger(),"乌龟%s生成成功！", name.c_str());
    }

  } else {
    RCLCPP_INFO(client->get_logger(),"请求异常");
  }

  // 释放资源。
  rclcpp::shutdown();
  return 0;
}
