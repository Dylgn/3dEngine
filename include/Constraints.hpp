#pragma once

#include "Entity.hpp"

struct Constraint {
    union {
        struct {
            Entity *first, *second;
        } entity;
        Entity *entities[2];
    };

    union {
        struct {
            V3d *first, *second;
        } point;
        V3d *points[2];
    }; // World coordinates on each entity's body involved in constraint

    virtual void solve(float h) = 0;
};

namespace Constraints {

struct Distance : public Constraint {
    float distance; // Constraint distance
    float compliance; // Inverse of stiffness, value of 0 is infinitely stiff
    float *lambda; // TODO add desc : multiplier

    void solve(float h) override;

private:
    float getGeneralizedInverseMass(unsigned i, const V3d &r, const V3d &n, float c) const;
};

struct Angular : public Constraint {
    float compliance; // Stiffness of the constraint
    float lambda_pos; // TODO add desc : multiplier

    void apply(float h, const V3d &delta_q, float &lambda);
};

// Mutual Orientation
struct Mutual : public Angular {
private:
    void solve(float h) override;
};

// Hinge joints
struct Hinge : public Angular {
    union {
        struct {
            V3d *first, *second;
        } align_axis;
        V3d *align_axes[2];
    }; // Local alligned axis
    float *lambda_align;
    
    union {
        struct {
            V3d *first, *second;
        } limit_axis;
        V3d *limit_axes[2];
    }; // Local limit axis
    float *lambda_limit;

    float lower_limit;
    float upper_limit;
private:
    void solve(float h) override;
};

// Spherical Joints
struct Spherical : public Angular {
    struct Limits {
        float lower_limit;
        float upper_limit;
    };

    union {
        struct {
            V3d *first, *second;
        } swing_axis;
        V3d *swing_axes[2];
    }; // Local swing axis
    float *lambda_swing;
    Limits swing_limtis;

    union {
        struct {
            V3d *first, *second;
        } twist_axis;
        V3d *twist_axes[2];
    }; // Local swing axis
    float *lambda_twist;
    Limits twist_limits;

private:
    void solve(float h) override;
};

};

