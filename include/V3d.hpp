#pragma once

struct V3d {
    float x = 0;
    float y = 0;
    float z = 0;
    float w = 1;

    V3d operator+(const V3d &o) const;
    V3d operator-(const V3d &o) const;
    V3d operator*(float k) const;
    V3d operator/(float k) const;

    float dotProd(const V3d &o) const;

    V3d crossProd(const V3d &o) const;

    float length() const;
    
    V3d normalize() const;

    V3d opposite() const;

    static const V3d unit_x;
    static const V3d unit_y;
    static const V3d unit_z;
};