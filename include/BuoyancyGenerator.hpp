#pragma once

#include "ParticleForceGenerator.hpp"

class BuoyancyGenerator: public ParticleForceGenerator {
    float max_depth; // Max submersion depth before it reaches max buoyance force
    float volume;
    float liquid_height; // Y-value of liquid (assumes parallel to XZ-plane
    float liquid_density; // ex water is 1000kg/m3

    public:
        BuoyancyGenerator(float max_depth, float volume, float liquid_height, float liquid_density = 1000.0f);

        virtual void UpdateForce(Particle *p, float elapsed_time);
};