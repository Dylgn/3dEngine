#include "GravityGenerator.hpp"

GravityGenerator::GravityGenerator(const V3d &gravity): gravity{gravity} {}

void GravityGenerator::UpdateForce(Particle *p, float elapsed_time) {
    if (p->GetInverseMass() == 0) return; // Infinite mass
    else p->ApplyForce(gravity * p->GetMass());
}
