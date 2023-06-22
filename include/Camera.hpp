#pragma once

#include "V3d.hpp"
#include "M4x4.hpp"

struct Camera {
    M4x4 mat_proj;
    V3d pos;
    V3d look_dir;
    float theta = 0;
    float yaw = 0;
};