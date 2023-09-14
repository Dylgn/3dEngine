#pragma once

#include <vector>
#include "Rigidbody.hpp"
#include "ForceGenerator.hpp"

class World {
    public:
        typedef std::vector<Rigidbody*> Rigidbodies;

    protected:
        Rigidbodies bodies; // Rigidbodies being simulated

        ForceRegistry registry;

    public:
        void StartFrame(); // Clears force and torque accumulators for bodies in world from pevious frame

        void Run(float elapsed_time);

        void Integrate(float elapsed_time);
};