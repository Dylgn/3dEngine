#pragma once

#include "V3d.hpp"

struct Quaternion {
    union {
        struct {
            float x, y, z, w;
        };
        float data[4];
    };

    Quaternion(float x, float y, float z, float w);
    Quaternion(V3d axis, float angle);

    Quaternion operator-() const;
};