#pragma once

#include "Particle.hpp"

struct ParticleContact {
    Particle *particles[2]; // Particles involved in collision
    float restitution; // Normal restitution coefficient at contact
    V3d normal; // Collision Normal
    float depth; // Collision depth

    void Resolve(float elapsed_time); // Resolves velocity and collision
    float CalculateSeparatingVelocity() const; // Calculates separating velocity

    private:
        void ResolveVelocity(float elapsed_time); // Resolves velocity
        void ResolveCollision(float elapsed_time); // Resolves collision
};