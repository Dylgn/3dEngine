#pragma once

#include "Particle.hpp"
#include "Contact.hpp"

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