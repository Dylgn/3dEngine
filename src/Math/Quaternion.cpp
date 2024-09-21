#include <cmath>
#include "Quaternion.hpp"

Quaternion::Quaternion(float x, float y, float z, float w):
    x{x}, y{y}, z{z}, w{w} {}

Quaternion::Quaternion(V3d axis, float angle)
{
    axis = axis.normalize();

    float s_angle = sinf(angle / 2.0);

    this->x = axis.x * s_angle;
    this->y = axis.y * s_angle;
    this->z = axis.z * s_angle;
    this->w = cosf(angle / 2.0);
}

Quaternion Quaternion::operator-() const 
{
    return {
        -this->data[0], -this->data[1], -this->data[2],
        this->data[3]
    };
}
