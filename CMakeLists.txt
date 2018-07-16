cmake_minimum_required(VERSION 3.5)
project(rttest_cpp)

# Default to C++14
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 14)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(object_msgs REQUIRED)
find_package(object_analytics_msgs REQUIRED)

add_executable(pointcloud2 src/pointcloud2.cpp)
ament_target_dependencies(pointcloud2
  rclcpp std_msgs object_msgs object_analytics_msgs sensor_msgs)

add_executable(localization src/localization.cpp)
ament_target_dependencies(localization
  rclcpp std_msgs object_msgs object_analytics_msgs sensor_msgs)

add_executable(detection src/detection.cpp)
ament_target_dependencies(detection
  rclcpp std_msgs object_msgs object_analytics_msgs sensor_msgs)

add_executable(tracking src/tracking.cpp)
ament_target_dependencies(tracking
  rclcpp std_msgs object_msgs object_analytics_msgs sensor_msgs)

install(TARGETS
  pointcloud2
  localization
  detection
  tracking
  DESTINATION lib/${PROJECT_NAME})

ament_package()
