#pragma once

#include "ParticleForceGenerator.hpp"

class BungeeGenerator: public ParticleForceGenerator {
    Particle *other;
    float spring_const;
    float rest_length;

    public:
        BungeeGenerator(Particle *other, float spring_const, float rest_length);

        virtual void UpdateForce(Particle *p, float elapsed_time);
};