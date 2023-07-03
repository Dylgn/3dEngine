#pragma once

#include "Collider.hpp"
#include "Mesh.hpp"

class Object {
    Collider *m_collider;
    Mesh mesh;
    V3d m_force;
    V3d m_velocity;
    float m_mass;
};