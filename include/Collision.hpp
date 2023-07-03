#pragma once

#include "Collider.hpp"

namespace Collision {
    /** Gilbert-Johnson-Keerthi distance algorithm for determining if Colliders are colliding.
     * @returns True if Colliders are colliding, False otherwise
    */
    bool GJK(const Collider *a, const Collider *b);
}