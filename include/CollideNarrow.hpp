#pragma once

#include "Rigidbody.hpp"
#include "Contact.hpp"
#include "M4x4.hpp"

struct Primitive {
    Rigidbody *body;
    M3x4 offset;

    V3d getAxis(unsigned index) const;

    M3x4 transform;
};

struct Sphere : public Primitive {
    float radius;
};

struct Plane : public Primitive {
    V3d normal;
    float offset; // Distance of the plane from the origin
};

struct Box : public Primitive {
    V3d half_size;
};

/** Info to help detector build contact data. */
struct CollisionData {
    Contact *contacts; // Contact array
    unsigned size; // Contact array size
    unsigned free_slots; // Remaining slots in array
    float friction;
    float restitution;

    /** Notifies the data that the given # of contacts have been added. */
    void AddContacts(unsigned count);
};

namespace CollisionDetector {
    /** Sphere-sphere collision */
    unsigned sphereSphere(const Sphere&, const Sphere&, CollisionData *data);
    /** Sphere-half-space collision. Treats whole region on back side of plane as solid. */
    unsigned sphereHalfSpace(const Sphere &sphere, const Plane &plane, CollisionData *data);
    /** Sphere-plane collision. Does not treat whole region on back side of plane as solid. */
    unsigned spherePlane(const Sphere &sphere, const Plane &plane, CollisionData *data);
    /** Box-half-space collision. Treats whole region on back side of plane as solid. */
    unsigned boxHalfSpace(const Box &box, const Plane &plane, CollisionData *data);
    /** Box-sphere collision. */
    unsigned boxSphere(const Box &box, const Sphere &sphere, CollisionData *data);
    /** Box-box collision. */
    unsigned boxBox(const Box&, const Box&, CollisionData *data);
};