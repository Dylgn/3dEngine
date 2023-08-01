#pragma once

#include "Transform.hpp"
#include "Collider.hpp"

class Body {
    protected:
        Collider *collider;
    public:
        Body(Collider *collider): collider{collider} {}
        virtual ~Body() { delete collider; }
        
        virtual void Move(const V3d &dir) { collider->Move(dir); }

        V3d FurthestPointIn(const V3d &dir) { return collider ? collider->FurthestPointIn(dir) : V3d::origin; }
        
        friend class Object;
};