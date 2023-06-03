#include <cmath>
#include "V3d.hpp"
#include "MathUtility.hpp"

V3d V3d::operator+(V3d o) {
    return {
        this->x + o.x,
        this->y + o.y,
        this->z + o.z
    };
}
V3d V3d::operator-(V3d o) {
    return {
        this->x - o.x,
        this->y - o.y,
        this->z - o.z
    };
}
V3d V3d::operator*(float k) {
    return {
        this->x * k,
        this->y * k,
        this->z * k
    };
}
V3d V3d::operator/(float k) {
    return {
        this->x / k,
        this->y / k,
        this->z / k
    };
}

float V3d::dotProd(V3d o) {
    return MathUtil::DotProd(*this, o);
}

V3d V3d::crossProd(V3d o) {
    return MathUtil::CrossProd(*this, o);
}

float V3d::length() {
    return sqrtf(dotProd(*this));
}

V3d V3d::normalize() {
    float l = length();
    return { x / l, y / l, z / l };
}