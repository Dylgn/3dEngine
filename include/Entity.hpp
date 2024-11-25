#pragma once

#include "V3d.hpp"
#include "M3x3.hpp"
#include "Quaternion.hpp"
#include "Mesh.hpp"

struct Entity {
    V3d pos;
    V3d velocity;
    float mass;

    Quaternion rot;
    V3d angular_velocity;
    M3x3 inertia_tensor;

    struct {
        V3d pos;
        Quaternion rot;
    } prev;

    struct {
        M3x3 inertia_tensor;
    } inv;

    V3d ToWorld(const V3d &v) const; // Converts coordinate relative to to entity (local) to world coordinate
    V3d ToLocal(const V3d &v) const; // Converts world coordinate relative to entity (local)
    float GetInverseMass() const;
};