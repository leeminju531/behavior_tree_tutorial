cmake_minimum_required(VERSION 3.0.2)
project(behavior_tree_tutorial)


find_package(catkin REQUIRED COMPONENTS
  behaviortree_cpp_v3
  roscpp
  rospy
  std_msgs
)


catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES behavior_tree_tutorial
#  CATKIN_DEPENDS behaviortree_cpp_v3 roscpp rospy std_msgs
#  DEPENDS system_lib
)

## Specify additional locations of header files
## Your package locations should be listed before other locations
include_directories(
# include
  ${catkin_INCLUDE_DIRS}
)
add_executable(tutorial1 src/Tutorial1_Create_a_tree/tutorial1.cpp)
target_link_libraries(tutorial1 ${catkin_LIBRARIES})