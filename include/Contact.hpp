#pragma once

#include "Particle.hpp"

struct ParticleContact {
    Particle *particles[2]; // Particles involved in collision
    float restitution; // Normal restitution coefficient at contact
    V3d normal; // Collision Normal
    float depth; // Collision depth

    friend class ParticleContactResolver;

    protected:
        void Resolve(float elapsed_time); // Resolves velocity and collision
        float CalculateSeparatingVelocity() const; // Calculates separating velocity

    private:
        void ResolveVelocity(float elapsed_time); // Resolves velocity
        void ResolvePenetration(float elapsed_time); // Resolves collision
};

class ParticleContactResolver {
    protected:
        unsigned iterations; // Max iterations
        unsigned iterations_used; // Number of iterations used

    public:
        ParticleContactResolver(unsigned iterations);

        /** Set max number of iterations */
        void SetIterations(unsigned iterations);
        /** Resolves penetration and velocity for particle contacts */
        void ResolveContacts(ParticleContact *contacts, unsigned count, float elapsed_time);
};