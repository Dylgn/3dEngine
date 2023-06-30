#pragma once

#include "Interval.hpp"
#include "V3d.hpp"

class BoundingBox {
    Interval x;
    Interval y;
    Interval z;
    public:
        BoundingBox();
        BoundingBox(Interval x, Interval y, Interval z);
        BoundingBox(Interval bounds[3]);
        
        bool Colliding(BoundingBox &b);
        
        float getCentreX();
        float getCentreY();
        float getCentreZ();

        void move(V3d dir);
};