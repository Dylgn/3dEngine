#pragma once

#include "V3d.hpp"

struct BoundingSphere {
    V3d center;
    float radius;
    
    BoundingSphere(const V3d &center, float radius);

    /** Creates a BoundingSphere that encloses the two given spheres. */
    BoundingSphere(const BoundingSphere &one, const BoundingSphere &two);

    /** Checks if overlaping with given BoundingSphere */
    bool overlaps(BoundingSphere *other);

    /** Gets growth required to incorporate given bounding sphere */
    float getGrowth(const BoundingSphere &other) const;

    /** Gets the volume of the sphere */
    float getVolume() const;
};