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

Quaternion Quaternion::operator+(const Quaternion &q) const
{
    return {
        this->x + q.x,
        this->y + q.y,
        this->z + q.z,
        this->w + q.w
    };
}

Quaternion Quaternion::operator-(const Quaternion &q) const
{
    return Quaternion(
        this->x - q.x,
        this->y - q.y,
        this->z - q.z,
        this->w - q.w
    );
}

Quaternion Quaternion::operator-() const 
{
    return {
        -this->data[0], -this->data[1], -this->data[2],
        this->data[3]
    };
}

Quaternion Quaternion::operator*(const Quaternion &q) const
{
    return {
        this->w * q.x + this->x * q.w + this->y * q.z - this->z * q.y,
        this->w * q.y + this->y * q.w + this->z * q.x - this->x * q.z,
        this->w * q.z + this->z * q.w + this->x * q.y - this->y * q.x,
        this->w * q.w - this->x * q.x - this->y * q.y - this->z * q.z
    };
}

Quaternion Quaternion::operator*(const V3d &v) const
{
    return {
        this->w * v.x + this->y * v.z - this->z *  v.y,
        this->w * v.y + this->z * v.x - this->x * v.z,
        this->w * v.z + this->x * v.y - this->y * v.x,
        -(this->x * v.x + this->y * v.y + this->z * v.z)
    };
}

Quaternion Quaternion::operator*(float k) const
{
    return {
        this->x * k,
        this->y * k,
        this->z * k,
        this->w * k
    };
}

V3d Quaternion::rotate(const V3d &v) const
{
    //Quaternion ret = (*this * v) * -(*this);
    //return {
    //    ret.x, ret.y, ret.z
    //};

    // https://gamedev.stackexchange.com/a/50545
    V3d u{this->x, this->y, this->z};
    float w = this->w;

    return u * 2.0f * u.dot(v) + v * (w * w - u.dot(u)) + u.cross(v) * 2.0f * w;
    
}

Quaternion Quaternion::inverse() const
{
    return -(*this);
}

Quaternion Quaternion::normalize() const
{
    float l = sqrtf(this->x * this->x + this->y * this->y + this->z * this->z + this-> w * this->w);
    return {
        this->x / l, this->y / l, this->z / l, this->w / l
    };
}