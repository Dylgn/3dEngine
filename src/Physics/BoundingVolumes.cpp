#include <cmath>
#include "BoundingVolumes.hpp"

BoundingSphere::BoundingSphere(const V3d &center, float radius): center{center}, radius{radius} {}

BoundingSphere::BoundingSphere(const BoundingSphere &one, const BoundingSphere &two) {
    V3d offset = two.center - one.center;
    float dist = offset.squareLength();
    float diff = two.radius - one.radius;

    // Check whether the larger sphere encloses the smaller one
    if (diff * diff >= dist) {
        if (one.radius > two.radius) {
            center = one.center;
            radius = one.radius;
        } else {
            center = two.center;
            radius = two.radius;
        }
    } else { // Partially overlapping
        dist = sqrtf(dist);
        radius = (dist + one.radius + two.radius) * (0.5f);

        // New center is based on one's center, moved toward two's center
        // proportional to spheres' radii
        center = one.center;
        if (dist > 0) {
            center += offset * ((radius - one.radius) / dist);
        }
    }
}

bool BoundingSphere::overlaps(BoundingSphere *other) {
    return (center - other->center).squareLength() < (radius + other->radius) * (radius + other->radius);
}

float BoundingSphere::getVolume() const {
    return (4.0f / 3.0f) * 3.14159f * radius * radius * radius; 
}
