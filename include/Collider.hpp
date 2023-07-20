#pragma once

#include "V3d.hpp"

/** Used to test collision between objects. Assumes a convex shape.*/
struct Collider {
    /** Move all vertices in given direction */
    virtual void Move(const V3d &dir) = 0;
    /** Gets center of collider */
    virtual V3d GetCenter() const = 0;
    /** Gets furthest point in given direction */
    virtual V3d FurthestPointIn(const V3d &dir) const = 0;
};