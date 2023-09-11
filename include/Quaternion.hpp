#pragma once

#include "V3d.hpp"

struct Quaternion {
    union {
        struct {
            float r; // real
            float i; // complex
            float j;
            float k;
        };
        float data[4];
    };

    void operator*=(const Quaternion &q);

    /** Normalizes the quaternion. */
    void Normalize();

    void RotateBy(const V3d &v);

    void AddScaledVector(const V3d &v, float scale);
};