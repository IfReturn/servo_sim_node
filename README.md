# Servo Sim - ROS2 Gimbal Simulation

A ROS2 package for simulating a servo gimbal (pan-tilt camera mount) using Gazebo.

## Overview

This package provides a complete simulation environment for a servo gimbal consisting of:
- Base platform
- Horizontal rotation cylinder (pan)
- Vertical rotation cylinder (tilt) 
- Camera module

## Requirements

- ROS2 (tested with humble)
- Gazebo Classic (11 or later)
- RViz2
- Standard ROS2 packages:
  - `robot_state_publisher`
  - `joint_state_publisher_gui`
  - `gazebo_ros`
  - `gazebo_ros2_control`
  - `controller_manager`
  - `position_controllers`
  - `joint_state_broadcaster`

## Installation

1. Clone this repository into your ROS2 workspace:
```bash
cd ~/ros2_ws/src
git clone https://github.com/IfReturn/servo_sim.git
```

2. Build the package:
```bash
# make sure you have sourced your ROS2 workspace
cd ~/ros2_ws
colcon build
source install/setup.bash
```

## Usage

```bash
ros2 launch servo_sim servo_sim.launch.py
```

## Manual Control

Send twist commands to control the gimbal:
```bash
ros2 topic pub /servo/cmd_vel geometry_msgs/msg/Twist "{angular: {y: 0.5, z: 0.3}}"
```

## Topics

- `/camera/image_raw` - Camera image stream
- `/servo/cmd_vel` - Twist commands for manual control
- `/joint_position_controller/commands` - Direct vertical joint position commands
- `/servo/command` - Command for the servo joints
- `/joint_states` - Current joint positions

## Package Structure

```
servo_sim/
├── CMakeLists.txt          # Build configuration
├── package.xml             # Package dependencies
├── config/                 # Launch files
│   └──servo_controllers.yaml # controller config
├── launch/                 # Launch files
│   └── servo_sim.launch.py # Basic simulation
├── model/                  # Robot models
│   └── servo.xacro         # Main robot description
├── msg/                    # Robot messages
│   └── ServoPosition.msg   # Main robot message
├── rviz/                   # RViz configurations
│   └── servo.rviz          # Default RViz config
├── src/                    # Source code
│   └── nodes/              # ROS2 nodes
│       └── servo_controller.cpp  # C++ controller
└── world/                  # Gazebo world files
    └── world.sdf           # Simulation world
```

## License

Apache 2.0

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request.

## TODO
- Test with Gazebo Classic integration
- Optimize control parameters for better performance 