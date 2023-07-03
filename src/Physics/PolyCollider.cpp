#include <cfloat>
#include "PolyCollider.hpp"
#include <iostream>

PolyCollider::PolyCollider(std::initializer_list<V3d> list) {
    for (auto v : list) {
        m_vertices.push_back(v);
    }
}

void PolyCollider::move(V3d dir) {
    for (auto &v : m_vertices) {
        v = v + dir;
    }
}

V3d PolyCollider::FurthestPointIn(V3d dir) const {
    V3d max_point;
    float max_dist = -FLT_MAX;

    // Loops through all vertices and gets furthest in 'dir' direction
    for (V3d v : m_vertices) {
        float dist = v.dotProd(dir);
        if (dist > max_dist) {
            max_dist = dist;
            max_point = v;
        }
    }

    return max_point;
}

void PolyCollider::print() {
    std::cout << "COLLIDER:" << std::endl;
    for (auto &v : m_vertices) {
        std::cout << v.x << " " << v.y << " " << v.z << std::endl;
    }
}
