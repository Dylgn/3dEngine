#pragma once

#include "Quaternion.hpp"
#include "V3d.hpp"

struct M3x4 {
    float data[12];
    
    float Get(int x, int y) const;
    void Set(int x, int y, float value);

    V3d operator*(const V3d &v) const;

    float GetDeterminant() const;
    M3x4 GetInverse() const;
    void Invert();

    /** Sets matrix to be rotation matrix corresponding to the given quaternion. */
    void SetAsOrientationPosOf(const Quaternion &q, const V3d &pos);

    /** Transforms given vector by this matrix */
    V3d Transform(const V3d &v) const;
    
    /** Transform given vector by transformational inverse of this matrix */
    V3d TransformInverse(const V3d &v) const;

    /** Transform given direction vector by transformational inverse of this matrix. */
    V3d TrasnformInverseDirection(const V3d &v) const;

    /** Transform given directioon vector by matrix. */
    V3d TransformDirection(const V3d &v) const;
    
    private:
        void SetAsInverseOf(const M3x4 &m);
};