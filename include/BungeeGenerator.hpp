#pragma once

#include "ForceGenerator.hpp"

class BungeeGenerator: public ForceGenerator {
    Particle *other;
    float spring_const;
    float rest_length;

    public:
        BungeeGenerator(Particle *other, float spring_const, float rest_length);

        virtual void UpdateForce(Particle *p, float elapsed_time);
};