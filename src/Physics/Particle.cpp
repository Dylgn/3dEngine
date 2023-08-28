#include "Particle.hpp"

void Particle::SetPos(V3d pos) {
    this->pos = pos;
}

V3d Particle::GetPos() {
    return pos;
}

void Particle::SetMass(float mass) {
    inverse_mass = 1.0f / mass;
}

float Particle::GetMass() {
    if (inverse_mass == 0) return -1.0f;
    else return 1.0f / inverse_mass;
}

void Particle::SetInverseMass(float inverse_mass) {
    this->inverse_mass = inverse_mass;
}

float Particle::GetInverseMass() {
    return inverse_mass;
}

void Particle::SetVelocity(V3d velocity) {
    this->velocity = velocity;
}

V3d Particle::GetVelocity() {
    return force;
}

V3d Particle::GetAcceleration() {
    return acceleration;
}

void Particle::ApplyForce(const V3d &force) {
    this->force += force;
}

void Particle::ClearForces() {
    force = V3d::origin;
}

void Particle::Integrate(float elapsed_time) {
    if (elapsed_time == 0) return;

    pos = pos + (velocity * elapsed_time);
    velocity = (velocity + (force * inverse_mass)) * elapsed_time;
}
