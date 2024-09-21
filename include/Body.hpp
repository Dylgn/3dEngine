#pragma once

#include "Transform.hpp"
#include "PolyCollider.hpp"

class Body {
    protected:
        PolyCollider *collider;
    public:
        Body(PolyCollider *collider): collider{collider} {}
        virtual ~Body() { delete collider; }
        
        virtual void Move(const V3d &dir) { collider->Move(dir); }

        V3d FurthestPointIn(const V3d &dir) { return collider ? collider->FurthestPointIn(dir) : V3d::origin; }
        
        friend class Object;
};

class Rbody: public Body {
    V3d force;  
    V3d velocity;
    float mass = 1.0f;
    public:
        Rbody(PolyCollider *collider);
        /** Calculates velocity direction */
        V3d CalculateVelocity(const float &elapsed_time);
        /** Set velocity */
        void SetVelocity(const V3d &dir);
        /** Get velocity */
        V3d GetVelocity();
        /** Accelerates body in given direction (F=ma)*/
        void Accelerate(const V3d &dir);
        /** Applies force to body */
        void ApplyForce(const V3d &dir);
        /** Sets force of body */
        void SetForce(const V3d &dir);
        /** Get force */
        V3d GetForce();
        /** Get mass */
        float GetMass();
};