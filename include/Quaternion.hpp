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

    Quaternion operator+(const Quaternion &q) const;
    Quaternion operator-(const Quaternion &q) const;

    Quaternion operator-() const;

    Quaternion operator*(const Quaternion &q) const;
    // Warning: not to be confused with Quaternion::rotate()!
    Quaternion operator*(const V3d &v) const;
    Quaternion operator*(float k) const;

    V3d rotate(const V3d &v) const;
    Quaternion inverse() const;

    Quaternion normalize() const;

    V3d cross(const V3d &v) const;
};