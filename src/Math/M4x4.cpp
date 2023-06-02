#include "V3d.hpp"
#include "MathUtility.hpp"

M4x4 M4x4::operator*(M4x4 o) {
    M4x4 ret;
    for (int c = 0; c < 4; ++c) {
        for (int r = 0; r < 4; ++r) {
            ret.m[r][c] = 
            this->m[r][0] * o.m[0][c] + 
            this->m[r][1] * o.m[1][c] + 
            this->m[r][2] * o.m[2][c] + 
            this->m[r][3] * o.m[3][c];
        }
    }
    return ret;
}

V3d M4x4::operator*(V3d &v) {
    return MathUtil::Mat4x4VecMult(*this, v);
}

