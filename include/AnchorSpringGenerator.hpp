#pragma once

#include "ForceGenerator.hpp"

// Links one end of spring with fixed point
class AnchorSpringGenerator: public ForceGenerator {
    protected:
        V3d *anchor; // Location of other end of spring
        float spring_const;
        float rest_length;
    public:
        AnchorSpringGenerator(V3d *anchor, float spring_const, float rest_length);

        virtual void UpdateForce(Particle *p, float elapsed_time);
};