#include <cmath>
#include "Quaternion.hpp"

void Quaternion::operator*=(const Quaternion &q) {
    Quaternion t = *this;
    r = t.r * q.r - t.i * q.i - t.j * q.j - t.k * q.k;
    i = t.r * q.i + t.i * q.r + t.j * q.k - t.k * q.j;
    j = t.r * q.j + t.j * q.r + t.k * q.i - t.i * q.k;
    k = t.r * q.k + t.k * q.r + t.i * q.j - t.j * q.i;
}

void Quaternion::Normalize()
{
    float d = r * r + i * i + j * j + k * k;

    // Zero-length == no-rotation
    if (d == 0) {
        r = 1;
        return;
    }

    d = 1.0f / sqrtf(d);
    r *= d;
    i *= d;
    j *= d;
    k *= d;
}

void Quaternion::RotateBy(const V3d &v) {
    Quaternion q{0.0f, v.x, v.y, v.z};
    (*this) *= q;
}

void Quaternion::AddScaledVector(const V3d &v, float scale) {
    Quaternion q{0.0f, v.x * scale, v.y * scale, v.z * scale};
    q *= *this;
    r += q.r * 0.5f;
    i += q.i * 0.5f;
    j += q.j * 0.5f;
    k += q.k * 0.5f;
}
