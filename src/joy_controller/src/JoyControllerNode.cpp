#include "joy_controller/JoyControllerNode.hpp"

JoyController::JoyController(const std::string node_name) : Node(node_name)
{
    this->shape_pub = this->create_publisher<std_msgs::msg::Int32>("/shape_cmd", rclcpp::QoS(10));
    this->velocity_pub = this->create_publisher<geometry_msgs::msg::Point>("/velocity_cmd", rclcpp::QoS(10));

    this->joyReader = std::make_shared<JoystickReader>();
    this->joyData = std::make_shared<Data>();
    joyData->axis = &joyReader->axis;
    joyData->button = &joyReader->button;

    this->timer = create_wall_timer(std::chrono::milliseconds(5), [this]()
                                    { this->doWork(); });
}

void JoyController::update()
{
    joyData->spin();
    geometry_msgs::msg::Point msg;
    msg.x = joyData->velocity_x;
    msg.y = joyData->velocity_y;
    msg.z = joyData->velocity_theta;
    velocity_pub->publish(msg);

    std_msgs::msg::Int32 shape;
    shape.data = joyData->shape_;
    shape_pub->publish(shape);
}
void JoyController::doWork()
{
    std::string joystickDevice = "/dev/input/js";

    if (not joyReader->isDeviceAvailable())
    {
        std::cerr << "Joystick device unavailable: " << joyReader->device_ << std::endl;
        for (uint8_t i = 0; i < 32; ++i)
        {
            joyReader->stop();
            joyReader->device_ = joystickDevice + std::to_string(i);
            joyReader->start();
            if (joyReader->running_)
                break;
        }
    }
    else
    {
        this->joyReader->readJoystick(std::bind(&JoyController::update, this));
    }
}