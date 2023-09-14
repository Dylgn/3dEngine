#pragma once

#include <vector>
#include "Particle.hpp"

struct ParticleForceGenerator {
    virtual void UpdateForce(Particle *p, float elapsed_time) = 0;
};

class ParticleForceRegistry {
    protected:
        struct ForceRegistration {
            Particle *p;
            ParticleForceGenerator *pfg;
        };

        typedef std::vector<ForceRegistration> Registry;
        Registry registrations;
    public:
        /** Registers given particle and force generator pair. */
        void Add(Particle *p, ParticleForceGenerator *pfg);

        /** Removes given particle and force generator pair. No effect if pair is not registered */
        void Remove(Particle *p, ParticleForceGenerator *pfg);

        /** Clears all registrations from registry. */
        void Clear();

        /** Update particles in registry with their corresponding force generators. */
        void Update(float elapsed_time);
};