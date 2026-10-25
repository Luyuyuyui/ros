cmake_minimum_required(VERSION 3.8)
project(test_action)

# 查找依赖
find_package(ament_cmake REQUIRED)
find_package(rclpy REQUIRED)
find_package(rosidl_default_generators REQUIRED)

# 生成动作接口
rosidl_generate_interfaces(${PROJECT_NAME}
  "action/FileUpload.action"
  DEPENDENCIES action_msgs
)

# 安装Python包
install(DIRECTORY test_action/
  DESTINATION lib/python3.10/site-packages/
)

# 安装可执行脚本
install(PROGRAMS
  scripts/file_upload_server.py
  scripts/file_upload_client.py
  DESTINATION lib/${PROJECT_NAME}
)

ament_package()
