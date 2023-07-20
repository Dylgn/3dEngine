#pragma once

#include "V3d.hpp"

struct Transform {
    V3d pos = V3d::origin;
    float yaw = 0;
    float pitch = 0;
    float roll = 0;
};