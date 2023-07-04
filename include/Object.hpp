#pragma once

#include "Collider.hpp"
#include "Mesh.hpp"

struct Object {
    Collider *m_collider;
    Mesh m_mesh;
    V3d m_force;
    V3d m_velocity;
    float m_mass;
};