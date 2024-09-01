#include "joy_controller/joystick.hpp"
#include "icecream.hpp"

JoystickReader::JoystickReader()
{
    for (int i = Axis_enum::AXIS_L_HORIZONTAL; i != Axis_enum::AXIS_DPAD_VERTICAL + 1; ++i)
    {
        axis.push_back(0);
    }
    for (int i = Button_enum::BUTTON_X; i != Button_enum::BUTTON_R3 + 1; ++i)
    {
        button.push_back(0);
    }
    IC();
}

void JoystickReader::start()
{
    fd_ = open(device_.c_str(), O_RDONLY | O_NONBLOCK);
    if (fd_ < 0)
    {
        std::cerr << "Failed to open joystick device: " << device_ << std::endl;
        return;
    }

    running_ = true;
}

void JoystickReader::stop()
{
    running_ = false;
    if (fd_ >= 0)
    {
        close(fd_);
        fd_ = -1;
    }
}

bool JoystickReader::isDeviceAvailable() const
{
    return access(device_.c_str(), F_OK) != -1;
}

void JoystickReader::readJoystick(std::function<void()> update)
{

    struct js_event event;
    ssize_t bytesRead;
    while ((bytesRead = read(fd_, &event, sizeof(event))) > 0)
    {
        if (event.type & JS_EVENT_AXIS or event.type & JS_EVENT_BUTTON)
        {
            if (event.type & JS_EVENT_BUTTON)
            {
                button[event.number] = event.value;
            }
            else if (event.type & JS_EVENT_AXIS)
            {
                short offset = 0;
                if (event.number == AXIS_L_HORIZONTAL or event.number == AXIS_L_VERTICAL or event.number == AXIS_R_HORIZONTAL or event.number == AXIS_R_VERTICAL)
                    offset = -258;
                if (event.number == Axis_enum::AXIS_R_TRIGGER or event.number == Axis_enum::AXIS_L_TRIGGER)
                    offset = std::numeric_limits<short>::max();
                axis.at(event.number) = event.value + offset;
                // IC(event.number);
            }
            IC(event.type);
            IC(button, axis);

            update();
        }
    }

    if (bytesRead == -1 && errno == ENODEV)
    {
        // Joystick device was unplugged
        std::cerr << "Joystick device unplugged: " << device_ << std::endl;
        stop();
    }
}