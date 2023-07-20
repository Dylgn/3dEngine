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

        friend class Object;
};