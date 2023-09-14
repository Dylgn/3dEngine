#pragma once

#include "ParticleForceGenerator.hpp"

// Spring that links two objects
class SpringGenerator: public ParticleForceGenerator {
    Particle *other; // Particle at other end of spring
    float spring_const; // Spring constant
    float rest_length; // Rest length of spring

    public:
        SpringGenerator(Particle *other, float spring_const, float rest_length);

        virtual void UpdateForce(Particle *p, float elapsed_time);
};