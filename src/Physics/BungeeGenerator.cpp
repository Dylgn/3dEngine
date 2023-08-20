#include "BungeeGenerator.hpp"

BungeeGenerator::BungeeGenerator(Particle *other, float spring_const, float rest_length): other{other}, spring_const{spring_const}, rest_length{rest_length} {}

void BungeeGenerator::UpdateForce(Particle *p, float elapsed_time) {
    V3d force = p->GetPos();
    force += -other->GetPos();

    // Check if compressed
    float magnitude = force.length();
    if (magnitude <= rest_length) return;

    // Calculate magnitude
    magnitude = spring_const * (rest_length - magnitude);

    // Apply force
    force = force.normalize() * -magnitude;
    p->ApplyForce(force);
}
