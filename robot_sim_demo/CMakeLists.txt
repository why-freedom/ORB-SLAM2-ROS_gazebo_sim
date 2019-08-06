cmake_minimum_required(VERSION 2.8.3)
project(robot_sim_demo)

set(CMAKE_CXX_STANDARD 11)
find_package(catkin REQUIRED COMPONENTS
	roscpp
	rospy
	std_msgs
	nav_msgs
	message_generation
	controller_manager
	gazebo_ros
	tf
)

catkin_package(
   CATKIN_DEPENDS urdf xacro
)

include_directories(
  include
  ${catkin_INCLUDE_DIRS}
)

add_executable(SubAndPub src/SubAndPub.cpp )
#必须添加add_dependencies，否则找不到自定义的msg产生的头文件
# add_dependencies(talker topic_demo_generate_messages_cpp)
target_link_libraries(SubAndPub ${catkin_LIBRARIES})


