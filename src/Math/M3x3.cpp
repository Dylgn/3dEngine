#include "M3x3.hpp"

float M3x3::Get(int x, int y) const {
    return data[3 * y + x];
}

void M3x3::Set(int x, int y, float value) {
    data[3 * y + x] = value;
}

V3d M3x3::operator*(const V3d &v) const {
    return V3d{
        v.x * data[0] + v.y * data[1] + v.z * data[2],
        v.x * data[3] + v.y * data[4] + v.z * data[5],
        v.x * data[6] + v.y * data[7] + v.z * data[8]
    };
}

M3x3 M3x3::GetTranspose() const {
    M3x3 ret;
    ret.SetAsTransposeOf(*this);
    return ret;
}

M3x3 M3x3::GetInverse() const {
    M3x3 ret;
    ret.SetAsInverseOf(*this);
    return ret;
}

void M3x3::Invert() {
    SetAsInverseOf(*this);
}

void M3x3::SetAsInverseOf(const M3x3 &m) {
    float p1 = m.data[0] * m.data[4];
    float p2 = m.data[0] * m.data[5];
    float p3 = m.data[1] * m.data[3];
    float p4 = m.data[2] * m.data[3];
    float p5 = m.data[1] * m.data[6];
    float p6 = m.data[2] * m.data[6];

    // Determinant
    float inv_det = p1 * m.data[8] - p2 * m.data[7] - p3 * m.data[8] + p4 * m.data[7] + p5 * m.data[5] - p6 * m.data[4];

    if (inv_det == 0) return;
    else inv_det = 1.0f / inv_det;

    data[0] = (m.data[4] * m.data[8] - m.data[5] * m.data[7]) * inv_det;
    data[1] = -(m.data[1] * m.data[8] - m.data[2] * m.data[7]) * inv_det;
    data[2] = (m.data[1] * m.data[5] - m.data[2] * m.data[4]) * inv_det;
    data[3] = -(m.data[3] * m.data[8] - m.data[5] * m.data[6]) * inv_det;
    data[4] = (m.data[0] * m.data[8] - p6) * inv_det;
    data[5] = -(p2-p4) * inv_det;
    data[6] = (m.data[3] * m.data[7] - m.data[4] * m.data[6]) * inv_det;
    data[7] = -(m.data[0] * m.data[7] - p5) * inv_det;
    data[8] = (p1-p3) * inv_det;
}

void M3x3::SetAsTransposeOf(const M3x3 &m) {
    data[0] = m.data[0];
    data[1] = m.data[3];
    data[2] = m.data[6];
    data[3] = m.data[1];
    data[4] = m.data[4];
    data[5] = m.data[7];
    data[6] = m.data[2];
    data[7] = m.data[5];
    data[8] = m.data[8];
}

M3x3 M3x3::Lerp(const M3x3 &a, const M3x3 &b, float prop) {
    M3x3 ret;
    for (unsigned i = 0; i < 9; ++i) {
        ret.data[i] = a.data[i] * (1 - prop) + b.data[i] * prop;
    }
    return ret;
}
