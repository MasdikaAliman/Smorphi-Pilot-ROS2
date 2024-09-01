#ifndef JOYSTICK_HPP
#define JOYSTICK_HPP

#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <linux/joystick.h>
#include <errno.h>
#include <vector>
#include "button.hpp"
#include <functional>

class JoystickReader
{
public:
    JoystickReader();
    void start();
    void stop();

    bool isDeviceAvailable() const;

    void readJoystick(std::function<void()> update);

public:
    std::string device_;
    int fd_;

    bool running_;

    uint8_t axis_count;
    uint8_t button_count;

    std::vector<int> axis;
    std::vector<bool> button;
};

#endif