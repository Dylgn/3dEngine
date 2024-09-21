#include "Body.hpp"

Rbody::Rbody(PolyCollider *collider): Body{collider} {}

V3d Rbody::CalculateVelocity(const float &elapsed_time) {
    velocity += (force / mass) * elapsed_time;
    return velocity;
}

void Rbody::SetVelocity(const V3d &dir) {
    velocity = dir;
}

V3d Rbody::GetVelocity() {
    return velocity;
}

void Rbody::Accelerate(const V3d &dir) {
    force += dir * mass;
}

void Rbody::ApplyForce(const V3d &dir) {
    force += dir;
}

void Rbody::SetForce(const V3d &dir) {
    force = dir;
}

V3d Rbody::GetForce() {
    return force;
}

float Rbody::GetMass() {
    return mass;
}