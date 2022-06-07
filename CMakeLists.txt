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

include_directories(
# include
  ${catkin_INCLUDE_DIRS}
)
add_executable(tutorial1 src/Tutorial1_create_a_tree/main.cpp)
target_link_libraries(tutorial1 ${catkin_LIBRARIES})

add_executable(tutorial2 src/Tutorial2_basic_ports/main.cpp)
target_link_libraries(tutorial2 ${catkin_LIBRARIES})

add_executable(tutorial3 src/Tutorial3_generic_ports/main.cpp)
target_link_libraries(tutorial3 ${catkin_LIBRARIES})

add_executable(tutorial4 src/Tutorial4_reactive_trees/main.cpp)
target_link_libraries(tutorial4 ${catkin_LIBRARIES})

add_executable(tutorial4_2 src/Tutorial4_reactive_trees/main2.cpp)
target_link_libraries(tutorial4_2 ${catkin_LIBRARIES})

add_executable(tutorial5 src/Tutorial5_subtrees_and_loggers/main.cpp)
target_link_libraries(tutorial5 ${catkin_LIBRARIES})

add_executable(tutorial6 src/Tutorial6_ports_remapping/main.cpp)
target_link_libraries(tutorial6 ${catkin_LIBRARIES})

add_executable(tutorial9 src/Tutorial9_coroutines/main.cpp)
target_link_libraries(tutorial9 ${catkin_LIBRARIES})