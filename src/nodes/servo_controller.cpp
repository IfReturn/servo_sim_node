#include <cstddef>
#include <rclcpp/publisher.hpp>
#include <rclcpp/rclcpp.hpp>
#include <rclcpp/subscription.hpp>
#include <std_msgs/msg/float64_multi_array.hpp>
#include <sensor_msgs/msg/joint_state.hpp>
#include <geometry_msgs/msg/twist.hpp>
#include <servo_sim/msg/servo_position.hpp>
class ServoController : public rclcpp::Node
{
public:
    ServoController() : Node("servo_controller")
    {
        servo_position_pub_ = this->create_publisher<servo_sim::msg::ServoPosition>(
            "/servo/position", 10);

        // Publishers for joint commands
        cmd_pub_ = this->create_publisher<std_msgs::msg::Float64MultiArray>(
            "/joint_position_controller/commands", 10);
        // Subscriber for joint states
        joint_state_position_sub_ = this->create_subscription<sensor_msgs::msg::JointState>(
            "/joint_states", 10,
            [this](const sensor_msgs::msg::JointState::UniquePtr msg) {
                   // Store current joint positions
                for (size_t i = 0; i < msg->name.size(); ++i)
                {
                    if (msg->name[i] == "horizontal_joint")
                    {
                        current_horizontal_ = msg->position[i];
                    }
                    else if (msg->name[i] == "vertical_joint")
                    {
                        current_vertical_ = msg->position[i];
                    }
                }
            });

        // Subscriber for servo control commands
        servo_position_sub_ = this->create_subscription<servo_sim::msg::ServoPosition>(
            "/servo/command", 10,
            [this](const servo_sim::msg::ServoPosition::UniquePtr msg) {
                target_horizontal_ = msg->horizontal;
                target_vertical_ = msg->vertical;
            });

        // Subscriber for twist commands (for manual control)
        twist_sub_ = this->create_subscription<geometry_msgs::msg::Twist>(
            "/servo/cmd_vel", 10,
            [this](const geometry_msgs::msg::Twist::UniquePtr msg) {
                // Convert twist to joint targets
                // Angular z controls horizontal rotation
                // Angular y controls vertical rotation
                target_horizontal_ += msg->angular.z * 0.1;  // Scale factor
                target_vertical_ += msg->angular.y * 0.1;    // Scale factor
                
                // Clamp values to joint limits
                target_horizontal_ = std::max(-M_PI, std::min(M_PI, target_horizontal_));
                target_vertical_ = std::max(-M_PI/2, std::min(M_PI/2, target_vertical_));
            });
        
        // Timer for periodic updates
        timer_ = this->create_wall_timer(
            std::chrono::milliseconds(50),
            [this]() {
                // Send joint commands
                auto position_msg = std_msgs::msg::Float64MultiArray();
                position_msg.data.resize(2);
                position_msg.data[0] = target_horizontal_;
                position_msg.data[1] = target_vertical_;
                cmd_pub_->publish(position_msg);
                // Publish current position
                auto servo_position_msg = servo_sim::msg::ServoPosition();
                servo_position_msg.horizontal = current_horizontal_;
                servo_position_msg.vertical = current_vertical_;
                servo_position_pub_->publish(std::move(servo_position_msg));
            });
        RCLCPP_INFO(this->get_logger(), "Servo Controller Node Started");
    }

private:
    // Publishers
    rclcpp::Publisher<servo_sim::msg::ServoPosition>::SharedPtr servo_position_pub_;

    rclcpp::Publisher<std_msgs::msg::Float64MultiArray>::SharedPtr cmd_pub_;

    // Subscribers
    rclcpp::Subscription<sensor_msgs::msg::JointState>::SharedPtr joint_state_position_sub_;

    rclcpp::Subscription<servo_sim::msg::ServoPosition>::SharedPtr servo_position_sub_;

    rclcpp::Subscription<geometry_msgs::msg::Twist>::SharedPtr twist_sub_;
    // Timer
    rclcpp::TimerBase::SharedPtr timer_;
    
    // State variables
    double current_horizontal_ = 0.0;
    double current_vertical_ = 0.0;
    double target_horizontal_ = 0.0;
    double target_vertical_ = 0.0;
};

int main(int argc, char** argv)
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<ServoController>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}
