#pragma once

#include "Quaternion.hpp"

struct M3x3 {
    float data[9];

    float Get(int x, int y) const;
    void Set(int x, int y, float value);

    M3x3 GetTranspose() const;
    M3x3 GetInverse() const;
    void Invert();

    /** Sets matrix to be rotation matrix corresponding to the given quaternion. */
    void SetAsOrientationOf(const Quaternion &q);

    private:
        void SetAsInverseOf(const M3x3 &m);
        void SetAsTransposeOf(const M3x3 &m);
};