#pragma once

#include <vector>
#include "Collider.hpp"

class PolyCollider: public Collider {
    std::vector<V3d> m_vertices;

    public:
        PolyCollider(std::initializer_list<V3d> list);
        /** Move all vertices in given direction */
        void move(V3d dir) override;
        /** Gets the furthest vertex in the given direction */
        V3d FurthestPointIn(V3d dir) const override;
};