#include <cstdio>
#include "joy_controller/JoyControllerNode.hpp"

int main(int argc, char **argv)
{
  rclcpp::init(argc, argv);

  auto node = std::make_shared<JoyController>("joy_ctrl");
  rclcpp::spin(node);

  rclcpp::shutdown();
  return 0;
}
