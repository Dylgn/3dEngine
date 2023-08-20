#include <cmath>
#include "SpringGenerator.hpp"

SpringGenerator::SpringGenerator(Particle *other, float spring_const, float rest_length): other{other}, spring_const{spring_const}, rest_length{rest_length} {}

void SpringGenerator::UpdateForce(Particle *p, float elapsed_time) {
    V3d force = p->GetPos();
    force += -other->GetPos();

    // Calculate force magnitude
    float magnitude = force.length();
    magnitude = std::abs(magnitude - rest_length);
    magnitude = magnitude * spring_const;

    // Apply force
    force = force.normalize() * -magnitude;
    p->ApplyForce(force);
}
