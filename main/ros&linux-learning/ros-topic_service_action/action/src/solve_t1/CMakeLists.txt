cmake_minimum_required(VERSION 3.0.2)
project(solve_t1)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  std_msgs
  actionlib
  actionlib_msgs
  message_generation
)

add_action_files(
  FILES
  ELEV.action
)

generate_messages(
  DEPENDENCIES
  std_msgs
  actionlib_msgs 
)

catkin_install_python(PROGRAMS
  scripts/client.py
  scripts/server.py
  DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)

catkin_package(
  CATKIN_DEPENDS 
  roscpp 
  rospy 
  std_msgs 
  actionlib 
  actionlib_msgs
)

install(DIRECTORY launch
   DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
)
