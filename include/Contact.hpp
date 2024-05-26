#pragma once

#include "V3d.hpp"
#include "Rigidbody.hpp"

/** Represents two bodies in contact. */
struct Contact {
    Rigidbody *body[2]; // Bodies involved in contact. Note 2nd body may be nullptr
    V3d point; // Position of contact
    V3d normal; // Direction of contact
    float penetration; // Depth of penetration of contact point
    float friction; // Lateral friction coefficient at contact
    float restitution; // Normal restitution coefficient at contact

    void SetBodyData(Rigidbody *, Rigidbody *, float friction, float restitution);
};