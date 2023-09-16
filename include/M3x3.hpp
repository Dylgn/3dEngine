#pragma once

#include "Quaternion.hpp"

struct M3x3 {
    float data[9];

    float Get(int x, int y) const;
    void Set(int x, int y, float value);

    V3d operator*(const V3d &v) const;

    M3x3 GetTranspose() const;
    M3x3 GetInverse() const;
    void Invert();

    /** Sets matrix to be rotation matrix corresponding to the given quaternion. */
    void SetAsOrientationOf(const Quaternion &q);

    /** Sets matrix to be inverse of given matrix. */
    void SetAsInverseOf(const M3x3 &m);

    /** Sets matrix to be transpose of given matrix. */
    void SetAsTransposeOf(const M3x3 &m);

    /** Interpolates a couple of matrices */
    static M3x3 Lerp(const M3x3 &a, const M3x3 &b, float prop);
};