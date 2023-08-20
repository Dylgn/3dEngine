#include "DragGenerator.hpp"

DragGenerator::DragGenerator(float k1, float k2): k1{k1}, k2{k2} {}

void DragGenerator::UpdateForce(Particle *p, float elapsed_time) {
    V3d force = p->GetVelocity();

    // Calculate drag coefficient
    float drag = force.length();
    drag = k1 * drag + k2 * drag * drag;

    // Apply force
    force = force.normalize() * -drag;
    p->ApplyForce(force);
}
