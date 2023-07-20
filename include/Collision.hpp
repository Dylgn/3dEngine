#pragma once

#include "Collider.hpp"
#include "Simplex.hpp"

namespace Collision {
    /** Gilbert-Johnson-Keerthi distance algorithm for determining if Colliders are colliding.
     * @returns True if Colliders are colliding, False otherwise
    */
    bool GJK(const Collider *a, const Collider *b);

    /** Gilbert-Johnson-Keerthi distance algorithm for determining if Colliders are colliding.
     * @returns True if Colliders are colliding, False otherwise
    */
    bool GJK(Simplex &vertices, const Collider *a, const Collider *b);

    /** Expanding Polytope Algorithm for determining the normal and depth of a collision
     * @returns Normal of collision with length equal to depth of collision
    */
    V3d EPA(const Simplex &simplex, const Collider *a, const Collider *b);

    /** Expanding Polytope Algorithm for determining the normal and depth of a collision
     * @returns Normal of collision with length equal to depth of collision
    */
    V3d EPA(const Collider *a, const Collider *b);
}