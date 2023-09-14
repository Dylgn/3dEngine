#pragma once

#include "ParticleForceGenerator.hpp"

class GravityGenerator: public ParticleForceGenerator {
    V3d gravity;

    public:
        GravityGenerator(const V3d &gravity = V3d{0.0f, -9.81f, 0.0f});

        virtual void UpdateForce(Particle *p, float elapsed_time);
};