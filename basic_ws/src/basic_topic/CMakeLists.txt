cmake_minimum_required(VERSION 3.8)
project(basic_topic)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# 找依赖
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rclcpp_components REQUIRED)
find_package(geometry_msgs REQUIRED)

# Publisher Component
add_library(publisher_component SHARED
  src/publisher_component.cpp
)

target_include_directories(publisher_component PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

ament_target_dependencies(publisher_component
  rclcpp
  rclcpp_components
  geometry_msgs
)

rclcpp_components_register_node(publisher_component
  PLUGIN "basic_topic::PublisherComponent"
  EXECUTABLE publisher_node
)

# Subscriber Component
add_library(subscriber_component SHARED
  src/subscriber_component.cpp
)

target_include_directories(subscriber_component PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

ament_target_dependencies(subscriber_component
  rclcpp
  rclcpp_components
  geometry_msgs
)

rclcpp_components_register_node(subscriber_component
  PLUGIN "basic_topic::SubscriberComponent"
  EXECUTABLE subscriber_node
)

# 安装库和可执行文件
install(TARGETS publisher_component subscriber_component
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
)

# 安装头文件
install(DIRECTORY include/
  DESTINATION include)

# 安装 launch 文件
install(DIRECTORY launch
  DESTINATION share/${PROJECT_NAME})

# 声明 ament 包
ament_package()
