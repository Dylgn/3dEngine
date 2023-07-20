#include "Rigidbody.hpp"

Rigidbody::Rigidbody(Collider *collider): Body{collider} {}

V3d Rigidbody::CalculateVelocity(const float &elapsed_time) {
    velocity += (force / mass) * elapsed_time;
    return velocity;
}

void Rigidbody::Accelerate(const V3d &dir) {
    force += dir * mass;
}

void Rigidbody::ApplyForce(const V3d &dir) {
    force += dir;
}

void Rigidbody::SetForce(const V3d &dir) {
    force = dir;
}
