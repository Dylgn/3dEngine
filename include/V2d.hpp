#pragma once

struct V2d {
    float u = 0;
    float v = 0;
    float w = 1;

    V2d operator+(V2d o);
    V2d operator-(V2d o);
    V2d operator*(float k);
    V2d operator/(float k);
};