#include "M3x4.hpp"

float M3x4::Get(int x, int y) const {
    return data[4 * y + x];
}

void M3x4::Set(int x, int y, float value) {
    data[4 * y + x] = value;
}

V3d M3x4::operator*(const V3d &v) const {
    return V3d{
        v.x * data[0] + v.y * data[1] + v.z * data[2] + data[3],
        v.x * data[4] + v.y * data[5] + v.z * data[6] + data[7],
        v.x * data[8] + v.y * data[9] + v.z * data[10] + data[11]
    };
}

float M3x4::GetDeterminant() const {
    return  data[8] * data[5] * data[2] +
            data[4] * data[9] * data[2] +
            data[8] * data[1] * data[6] -
            data[0] * data[9] * data[6] -
            data[4] * data[1] * data[10] +
            data[0] * data[5] * data[10];
}

M3x4 M3x4::GetInverse() const
{
    M3x4 ret;
    ret.SetAsInverseOf(*this);
    return ret;
}

void M3x4::Invert() {
    SetAsInverseOf(*this);
}

V3d M3x4::Transform(const V3d &v) const {
    return (*this) * v;
}

V3d M3x4::TransformInverse(const V3d &v) const {
    V3d tmp = v;
    tmp.x -= data[3];
    tmp.y -= data[7];
    tmp.z -= data[11];
    return V3d{
        tmp.x * data[0] + tmp.y * data[4] + tmp.z * data[8],
        tmp.x * data[1] + tmp.y * data[5] + tmp.z * data[9],
        tmp.x * data[2] + tmp.y * data[6] + tmp.z * data[10]
    };
}

V3d M3x4::TrasnformInverseDirection(const V3d &v) const {
    return V3d{
        v.x * data[0] + v.y * data[4] +v.z * data[8],
        v.x * data[1] + v.y * data[5] +v.z * data[9],
        v.x * data[2] + v.y * data[6] +v.z * data[10]
    };
}

V3d M3x4::TransformDirection(const V3d &v) const {
    return V3d{
        v.x * data[0] + v.y * data[1] + v.z * data[2],
        v.x * data[4] + v.y * data[5] + v.z * data[6],
        v.x * data[8] + v.y * data[9] + v.z * data[10]
    };
}

V3d M3x4::GetColumnVector(int i) const
{
    return V3d{data[i], data[i + 4], data[i + 8]};
}

void M3x4::SetAsInverseOf(const M3x4 &m)
{
    float inv_det = GetDeterminant();
    if (inv_det == 0) return;
    else inv_det = 1.0f / inv_det;

    data[0] = (-m.data[9] * m.data[6] + m.data[5] * m.data[10]) * inv_det;
    data[4] = (m.data[8] * m.data[6] - m.data[4] * m.data[10]) * inv_det;
    data[8] = (-m.data[8] * m.data[5] + m.data[4] * m.data[9] * m.data[15]) * inv_det;
    data[1] = (m.data[9] * m.data[2] - m.data[1] * m.data[10]) * inv_det;
    data[5] = (-m.data[8] * m.data[2] + m.data[0] * m.data[10]) * inv_det;
    data[9] = (m.data[8] * m.data[1] - m.data[0] * m.data[9] * m.data[15]) * inv_det;
    data[2] = (-m.data[5] * m.data[2] + m.data[1] * m.data[6] * m.data[15]) * inv_det;
    data[6] = (+m.data[4] * m.data[2] - m.data[0] * m.data[6] * m.data[15]) * inv_det;
    data[10] = (-m.data[4] * m.data[1] + m.data[0] * m.data[5] * m.data[15]) * inv_det;

    data[3] = (m.data[9] * m.data[6] * m.data[3] 
    - m.data[5] * m.data[10] * m.data[3]
    - m.data[9] * m.data[2] * m.data[7]
    + m.data[1] * m.data[10] * m.data[7]
    + m.data[5] * m.data[2] * m.data[11]
    - m.data[1] * m.data[6] * m.data[11]) * inv_det;
    data[7] = (-m.data[8]*m.data[6]*m.data[3]
    + m.data[4] * m.data[10] * m.data[3]
    + m.data[8] * m.data[2] * m.data[7]
    - m.data[0] * m.data[10] * m.data[7]
    - m.data[4] * m.data[2] * m.data[11]
    + m.data[0] * m.data[6] * m.data[11]) * inv_det;
    data[11] =(m.data[8]*m.data[5]*m.data[3]
    - m.data[4] * m.data[9] * m.data[3]
    - m.data[8] * m.data[1] * m.data[7]
    + m.data[0] * m.data[9] * m.data[7]
    + m.data[4] * m.data[1] * m.data[11]
    - m.data[0] * m.data[5] * m.data[11]) * inv_det;
}
