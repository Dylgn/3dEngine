#include <cmath>
#include "V3d.hpp"
#include "MathUtility.hpp"

const V3d V3d::unit_x{1,0,0};
const V3d V3d::unit_y{0,1,0};
const V3d V3d::unit_z{0,0,1};

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

float V3d::dot(const V3d &o) const {
    return MathUtil::DotProd(*this, o);
}

V3d V3d::cross(const V3d &o) const {
    return MathUtil::CrossProd(*this, o);
}

float V3d::length() const {
    return sqrtf(dot(*this));
}

V3d V3d::normalize() const {
    float l = length();
    return { x / l, y / l, z / l };
}

V3d V3d::opposite() const {
    return {
        -this->x,
        -this->y,
        -this->z
    };
}