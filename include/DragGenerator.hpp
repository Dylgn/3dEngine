#pragma once

#include "ForceGenerator.hpp"

class DragGenerator: public ForceGenerator {
    // Coefficients for simplified model of drag
    float k1; // drag coefficient
    float k2; // squared drag coefficient

    public:
        DragGenerator(float k1, float k2);

        virtual void UpdateForce(Particle *p, float elapsed_time);
};