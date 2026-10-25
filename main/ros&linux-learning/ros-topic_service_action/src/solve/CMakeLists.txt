cmake_minimum_required(VERSION 3.0.2)
project(solve)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  std_msgs
  geometry_msgs
  actionlib
  message_generation
)

add_message_files(
  FILES
  match.msg
)

add_service_files(
  FILES
  Location.srv
  move.srv
)

generate_messages(
  DEPENDENCIES
  std_msgs
  geometry_msgs
)

catkin_install_python(PROGRAMS
  scripts/vision.py
  scripts/master.py
  scripts/move.py
  scripts/voice.py
  DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)

catkin_package(
  CATKIN_DEPENDS 
  roscpp 
  rospy 
  std_msgs 
  actionlib 
  message_runtime
)

install(DIRECTORY launch
   DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
)
