cmake_minimum_required(VERSION 3.0.2)
project(basic_shapes)

## Compile as C++11, supported in ROS Kinetic and newer
# add_compile_options(-std=c++11)

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED COMPONENTS
  geometry_msgs
  roscpp
  rospy
  sensor_msgs
  std_msgs
  visualization_msgs
)

###################################
## catkin specific configuration ##
###################################
## The catkin_package macro generates cmake config files for your package
## Declare things to be passed to dependent projects
## INCLUDE_DIRS: uncomment this if your package contains header files
## LIBRARIES: libraries you create in this project that dependent projects also need
## CATKIN_DEPENDS: catkin_packages dependent projects also need
## DEPENDS: system dependencies of this project that dependent projects also need
catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES basic_shapes
  CATKIN_DEPENDS geometry_msgs roscpp rospy sensor_msgs std_msgs visualization_msgs
  DEPENDS system_lib
)

###########
## Build ##
###########

## Specify additional locations of header files
## Your package locations should be listed before other locations
include_directories(
# include
  ${catkin_INCLUDE_DIRS}
)

## Add the executable
add_executable(basic_shapes_node_node src/basic_shapes_node.cpp)

## Specify libraries to link the executable with
target_link_libraries(basic_shapes_node_node
  ${catkin_LIBRARIES}
)

## Add dependencies
add_dependencies(basic_shapes_node_node ${catkin_EXPORTED_TARGETS})
