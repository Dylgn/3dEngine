#pragma once

#include "Entity.hpp"

struct Constraint {
    union {
        struct {
            Entity *first, *second;
        } entity;
        Entity *entities[2];
    };

    virtual void solve(float h) = 0;
};

namespace Constraints {

struct Distance : public Constraint {
    union {
        struct {
            V3d *first, *second;
        } point;
        V3d *points[2];
    }; // World coordinates on each entity's body involved in constraint

    float distance; // Constraint distance

    float compliance; // Inverse of stiffness, value of 0 is infinitely stiff

    float lambda; // TODO add desc : multiplier

    void solve(float h) override;

private:
    float getGeneralizedInverseMass(unsigned i, const V3d &r, const V3d &n, float c) const;
};

};

