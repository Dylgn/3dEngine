#pragma once

#include "ForceGenerator.hpp"

class GravityGenerator: public ForceGenerator {
    V3d gravity;

    public:
        GravityGenerator(const V3d &gravity = V3d{0.0f, -9.81f, 0.0f});

        virtual void UpdateForce(Particle *p, float elapsed_time);
};