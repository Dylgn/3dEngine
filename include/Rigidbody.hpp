#pragma once

#include "Body.hpp"

class Rigidbody: public Body {
    V3d force;  
    V3d velocity;
    float mass;
    public:
        Rigidbody(Collider *collider);
        /** Calculates velocity direction */
        V3d CalculateVelocity(const float &elapsed_time);
        /** Accelerates body in given direction (F=ma)*/
        void Accelerate(const V3d &dir);
        /** Applies force to body */
        void ApplyForce(const V3d &dir);
        /** Sets force of body */
        void SetForce(const V3d &dir);
};