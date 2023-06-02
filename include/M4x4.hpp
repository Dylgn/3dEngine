#pragma once

#include "V3d.hpp"

struct M4x4 {
    float m[4][4] = { 0 };

    M4x4 operator*(M4x4 o);
    V3d operator*(V3d &v);
};