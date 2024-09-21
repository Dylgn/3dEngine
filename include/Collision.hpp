#pragma once

#include "PolyCollider.hpp"
#include "Simplex.hpp"

namespace Collision {
    /** Gilbert-Johnson-Keerthi distance algorithm for determining if Colliders are colliding.
     * @returns True if Colliders are colliding, False otherwise
    */
    bool GJK(const PolyCollider *a, const PolyCollider *b);

    /** Gilbert-Johnson-Keerthi distance algorithm for determining if Colliders are colliding.
     * @returns True if Colliders are colliding, False otherwise
    */
    bool GJK(Simplex &vertices, const PolyCollider *a, const PolyCollider *b);

    /** Expanding Polytope Algorithm for determining the normal and depth of a collision
     * @returns Normal of collision with length equal to depth of collision
    */
    V3d EPA(const Simplex &simplex, const PolyCollider *a, const PolyCollider *b);

    /** Expanding Polytope Algorithm for determining the normal and depth of a collision
     * @returns Normal of collision with length equal to depth of collision
    */
    V3d EPA(const PolyCollider *a, const PolyCollider *b);
}