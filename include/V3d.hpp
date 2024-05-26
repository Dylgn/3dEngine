#pragma once

struct V3d {
    float x = 0;
    float y = 0;
    float z = 0;
    float w = 1;

    V3d operator+(const V3d &o) const;
    V3d operator-(const V3d &o) const;
    V3d operator*(const float &k) const;
    V3d operator/(const float &k) const;

    V3d &operator+=(const V3d &o);
    V3d &operator+=(const float &k);
    V3d &operator*=(const float &k);

    V3d operator-() const;

    float operator[](unsigned i) const;
    float &operator[](unsigned i);

    operator bool() const;

    float dot(const V3d &o) const;

    V3d cross(const V3d &o) const;

    float length() const;

    float squareLength() const;
    
    V3d normalize() const;

    static const V3d unit_x;
    static const V3d unit_y;
    static const V3d unit_z;
    static const V3d origin;
};