#ifndef DATA_HPP
#define DATA_HPP
#include "button.hpp"
#include <vector>

class Data
{
public:
    Data();
    void spin();

public:
    double velocity_x, velocity_y, velocity_theta;
    std::vector<int> *axis;
    std::vector<bool> *button;
    int shape_= -1;
    double velocity_max_x, velocity_max_y, velocity_max_theta;
};

#endif