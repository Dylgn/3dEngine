#include <cmath>
#include "V3d.hpp"
#include "MathUtility.hpp"

const V3d V3d::unit_x{1,0,0};
const V3d V3d::unit_y{0,1,0};
const V3d V3d::unit_z{0,0,1};
const V3d V3d::origin{0,0,0};

V3d V3d::operator+(const V3d &o) const {
    return {
        this->x + o.x,
        this->y + o.y,
        this->z + o.z
    };
}
V3d V3d::operator-(const V3d &o) const {
    return {
        this->x - o.x,
        this->y - o.y,
        this->z - o.z
    };
}
V3d V3d::operator*(const float &k) const {
    return {
        this->x * k,
        this->y * k,
        this->z * k
    };
}
V3d V3d::operator/(const float &k) const {
    return {
        this->x / k,
        this->y / k,
        this->z / k
    };
}

V3d &V3d::operator+=(const V3d &o) {
    this->x += o.x;
    this->y += o.y;
    this->z += o.z;
    return *this;
}

V3d &V3d::operator+=(const float &k) {
    this->x += k;
    this->y += k;
    this->z += k;
    return *this;
}

V3d &V3d::operator*=(const float &k) {
    this->x *= k;
    this->y *= k;
    this->z *= k;
    return *this;
}

V3d V3d::operator-() const {
    return {
        -this->x,
        -this->y,
        -this->z
    };
}

V3d::operator bool() const {
    return !(this->x == 0 && this->y == 0 && this->z == 0); 
}

float V3d::dot(const V3d &o) const {
    return MathUtil::DotProd(*this, o);
}

V3d V3d::cross(const V3d &o) const {
    return MathUtil::CrossProd(*this, o);
}

float V3d::length() const {
    return sqrtf(dot(*this));
}

float V3d::squareLength() const
{
    return dot(*this);
}

V3d V3d::normalize() const {
    float l = length();
    if (l == 0) return V3d::origin;
    else return { x / l, y / l, z / l };
}