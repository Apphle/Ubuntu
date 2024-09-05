cmake_minimum_required(VERSION 3.0.2)
project(gps_lla_to_utm)

## Compile as C++11, supported in ROS Kinetic and newer
add_compile_options(-std=c++11)

## Find catkin macros and libraries
find_package(catkin REQUIRED COMPONENTS
  geometry_msgs
  roscpp
  rospy
  sensor_msgs
  std_msgs
)

###################################
## catkin specific configuration ##
###################################
catkin_package(
  # INCLUDE_DIRS include
  # LIBRARIES gps_lla_to_utm
  CATKIN_DEPENDS geometry_msgs roscpp rospy sensor_msgs std_msgs
  DEPENDS system_lib
)

###########
## Build ##
###########

## Specify additional locations of header files
include_directories(
  # include
  ${catkin_INCLUDE_DIRS}
)

## Add the executable
add_executable(file_lla_to_utm_node src/file_lla_to_utm.cpp)

## Specify libraries to link the executable with
target_link_libraries(file_lla_to_utm_node
  ${catkin_LIBRARIES}
)

## Add dependencies
add_dependencies(file_lla_to_utm_node ${catkin_EXPORTED_TARGETS})
#################################################################################

## Add the executable
add_executable(gps_lla_to_utm_node_node src/gps_lla_to_utm_node.cpp)

## Specify libraries to link the executable with
target_link_libraries(gps_lla_to_utm_node_node
  ${catkin_LIBRARIES}
)

## Add dependencies
add_dependencies(gps_lla_to_utm_node_node ${catkin_EXPORTED_TARGETS})





