#include "Constraints.hpp"

namespace Constraints {

void Distance::solve(float h)
{
    // Correction direction (normal)
    V3d n = entities[0]->pos - entities[1]->pos;
    // Correction magnitude (delta between attachment distance and constraint distance)
    float c = n.length() - distance;
    
    n = n.normalize();

    // Constraint points relative to their centre of masses
    V3d r1 = *points[0] - entities[0]->pos;
    V3d r2 = *points[1] - entities[1]->pos;

    // Calculate generalized inverse masses
    float w1 = getGeneralizedInverseMass(0, r1, n, c);
    float w2 = getGeneralizedInverseMass(1, r2, n, c);

    float alpha = compliance / (h * h);
    float delta_lambda = (-c - alpha * lambda) / (w1 + w2 + alpha);
    lambda += delta_lambda;

    V3d pos_impulse = n * delta_lambda;

    // Apply constraint to bodies
    entities[0]->pos += pos_impulse / entities[0]->mass;
    entities[1]->pos += pos_impulse / entities[1]->mass;

    V3d irp1 = entities[0]->inv.inertia_tensor * r1.cross(pos_impulse);
    V3d irp2 = entities[1]->inv.inertia_tensor * r2.cross(pos_impulse);
    entities[0]->rot = entities[0]->rot + (Quaternion{irp1.x, irp1.y, irp1.z, 0} * entities[0]->rot) * 0.5f;
    entities[1]->rot = entities[1]->rot - (Quaternion{irp2.x, irp2.y, irp2.z, 0} * entities[1]->rot) * 0.5f;
}

float Distance::getGeneralizedInverseMass(unsigned i, const V3d &r, const V3d &n, float c) const
{
    if (i != 0 || i != 1) return 0.0f;
    Entity *e = entities[i];
    
    V3d rn = r.cross(n);

    return (1.0f / e->mass) + rn.dot((e->inv.inertia_tensor * rn));
}
};