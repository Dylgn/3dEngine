#pragma once

#include "V3d.hpp"

class Particle {
    protected:
        float inverse_mass; // The inverse of the mass of the object. (1 / Mass)
    public:
        V3d pos;
        V3d velocity;
        V3d acceleration;
        V3d force;
        float damping = 0.995f; // Used to simulate drag. Represents percentage of velocity kept after every physics update.

        /** Set particle position */
        void SetPos(V3d pos);
        /** Get particle position. */
        V3d GetPos();
        /** Set the mass of the particle. */
        void SetMass(float mass);
        /** Get mass of particle. */
        float GetMass();
        /** Set the mass of the particle using its inverse. */
        void SetInverseMass(float inverse_mass);
        /** Get inverse mass of particle. */
        float GetInverseMass();
        /** Set velocity of particle */
        void SetVelocity(V3d velocity);
        /** Get velocity of particle. */
        V3d GetVelocity();
        /** Get acceleration of particle */
        V3d GetAcceleration();
        /** Applies force to particle. */
        void ApplyForce(const V3d &force);
        /** Clears forces acting on particle. */
        void ClearForces();
        /** Integrate particle forward in time by given amount. */
        void Integrate(float elapsed_time);
};