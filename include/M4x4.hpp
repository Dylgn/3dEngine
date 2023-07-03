#pragma once

#include "V3d.hpp"

struct M4x4 {
    float m[4][4] = { 0 };

    M4x4 operator*(const M4x4 &o) const;
    V3d operator*(const V3d &v) const;
};