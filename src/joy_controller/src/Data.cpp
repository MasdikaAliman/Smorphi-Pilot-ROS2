#include "joy_controller/Data.hpp"
#include "icecream.hpp"
Data::Data() : velocity_x(0), velocity_y(0), velocity_theta(0), velocity_max_x(1.0), velocity_max_y(1.0), velocity_max_theta(4.2)
{
}

void Data::spin()
{
    // if (axis->at(AXIS_R_TRIGGER) != 0 or button->at(BUTTON_R1))
    {
        // IC(axis->at(AXIS_R_TRIGGER));
        double throttle = axis->at(AXIS_R_TRIGGER) / static_cast<double>(std::numeric_limits<unsigned short>::max());
        double brake = -1 * axis->at(AXIS_L_TRIGGER) / static_cast<double>(std::numeric_limits<unsigned short>::max());

        double scale = throttle; //+ brake;
        scale = 1.0 - scale;

        if (scale < 0.0)
            scale = 0.0;

        double angular = axis->at(AXIS_R_HORIZONTAL) / static_cast<double>(std::numeric_limits<short>::max());
        double linear_x = axis->at(AXIS_L_VERTICAL) / static_cast<double>(std::numeric_limits<short>::max());
        double linear_y = axis->at(AXIS_L_HORIZONTAL) / static_cast<double>(std::numeric_limits<short>::max());

        velocity_x = -velocity_max_x * linear_x * scale;
        velocity_y = velocity_max_y * linear_y * scale;
        velocity_theta = -(velocity_max_theta * angular);

        if (axis->at(AXIS_DPAD_VERTICAL) < 0)
            velocity_x = 1;
        else if (axis->at(AXIS_DPAD_VERTICAL) > 0)
            velocity_x = -1;
        if (axis->at(AXIS_DPAD_HORIZONTAL) < 0)
            velocity_y = -1;
        else if (axis->at(AXIS_DPAD_HORIZONTAL) > 0)
            velocity_y = 1;
    }
    // else
    // {
    //     velocity_x = 0;
    //     velocity_y = 0;
    //     velocity_theta = 0;
    // }
    // IC(velocity.x, velocity.y, velocity.theta);
    int shape = -1;
    if (button->at(BUTTON_X))
    {
        shape = 0;
    }
    else if (button->at(BUTTON_TRIANGLE))
    {
        shape = 1;
    }
    else if (button->at(BUTTON_O))
    {
        shape = 2;
    }
    else if (button->at(BUTTON_SQUARE))
    {
        shape = 3;
    }
    else if (button->at(BUTTON_OPTIONS))
    {
        shape = 4;
    }
    else if (button->at(BUTTON_SHARE))
    {
        shape = 5;
    }
    else if (button->at(BUTTON_L1))
    {
        shape = 6;
    }
    shape_ = shape;
}