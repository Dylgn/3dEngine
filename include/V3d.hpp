#pragma once

struct V3d {
    float x = 0;
    float y = 0;
    float z = 0;
    float w = 1;

    V3d operator+(V3d o);
    V3d operator-(V3d o);
    V3d operator*(float k);
    V3d operator/(float k);

    float dotProd(V3d o);

    V3d crossProd(V3d o);

    float length();
    
    V3d normalize();
};