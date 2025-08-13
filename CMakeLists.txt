cmake_minimum_required(VERSION 3.8)
project(servo_sim)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(robot_state_publisher REQUIRED)
find_package(xacro REQUIRED)
find_package(gazebo_ros2_control REQUIRED)
find_package(gazebo_ros REQUIRED)
find_package(controller_manager REQUIRED)
find_package(hardware_interface REQUIRED)
find_package(rosidl_default_generators REQUIRED)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
rosidl_generate_interfaces(${PROJECT_NAME}
  "msg/ServoPosition.msg"
)

ament_export_dependencies(rosidl_default_runtime)

rosidl_get_typesupport_target(cpp_typesupport_target
  ${PROJECT_NAME} rosidl_typesupport_cpp)
# Servo controller executable
add_executable(servo_controller src/nodes/servo_controller.cpp)
ament_target_dependencies(servo_controller
  rclcpp
  std_msgs
  sensor_msgs
  geometry_msgs
)
target_link_libraries(servo_controller "${cpp_typesupport_target}")

# Install directories
install(DIRECTORY
  launch
  model
  world
  rviz
  config
  DESTINATION share/${PROJECT_NAME}/
)


# Install executables
install(TARGETS
  servo_controller
  DESTINATION lib/${PROJECT_NAME}
)

#install compile commands for development tools
install(FILES
  ${PROJECT_BINARY_DIR}/compile_commands.json
  DESTINATION ${PROJECT_SOURCE_DIR}
)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()