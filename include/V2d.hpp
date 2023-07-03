#pragma once

struct V2d {
    float u = 0;
    float v = 0;
    float w = 1;

    V2d operator+(const V2d &o) const;
    V2d operator-(const V2d &o) const;
    V2d operator*(float k) const;
    V2d operator/(float k) const;
};