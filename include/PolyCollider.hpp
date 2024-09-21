#pragma once

#include <vector>
#include "V3d.hpp"

class PolyCollider {
    std::vector<V3d> m_vertices;

    public:
        PolyCollider(std::initializer_list<V3d> list);
        PolyCollider(std::vector<V3d> vertices);
        /** Move all vertices in given direction */
        void Move(const V3d &dir);
        /** Gets center of collider */
        V3d GetCenter() const;
        /** Gets the furthest vertex in the given direction */
        V3d FurthestPointIn(const V3d &dir) const;
};