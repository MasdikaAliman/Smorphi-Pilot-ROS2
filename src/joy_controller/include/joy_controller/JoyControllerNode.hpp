#ifndef JOYCONTROLLERNODE_HPP
#define JOYCONTROLLERNODE_HPP


#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/int32.hpp>
#include <geometry_msgs/msg/point.hpp>

#include "joy_controller/joystick.hpp"
#include "joy_controller/Data.hpp"
#include <functional>

class JoyController : public rclcpp::Node
{
public:
    JoyController(const std::string node_name);

private:
    void doWork();
    void update();

private:
    rclcpp::Publisher<geometry_msgs::msg::Point>::SharedPtr velocity_pub;
    rclcpp::Publisher<std_msgs::msg::Int32>::SharedPtr shape_pub;

    std::shared_ptr<JoystickReader> joyReader;
    std::shared_ptr<Data> joyData;
    rclcpp::TimerBase::SharedPtr timer;
};

#endif