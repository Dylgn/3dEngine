#pragma once

#include "V3d.hpp"

struct M3x4 {
    float data[12];
    
    float Get(int x, int y) const;
    void Set(int x, int y, float value);

    V3d operator*(const V3d &v) const;

    float GetDeterminant() const;
    M3x4 GetInverse() const;
    void Invert();

    /** Transforms given vector by this matrix */
    V3d Transform(const V3d &v) const;
    
    /** Transform given vector by transformational inverse of this matrix */
    V3d TransformInverse(const V3d &v) const;

    /** Transform given direction vector by transformational inverse of this matrix. */
    V3d TrasnformInverseDirection(const V3d &v) const;

    /** Transform given directioon vector by matrix. */
    V3d TransformDirection(const V3d &v) const;

    /** Gets a vector representing one column in the matrix */
    V3d GetColumnVector(int i) const;
    
    private:
        void SetAsInverseOf(const M3x4 &m);
};