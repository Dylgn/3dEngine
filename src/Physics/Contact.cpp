#include "Contact.hpp"

void ParticleContact::Resolve(float elapsed_time) {
    ResolveVelocity(elapsed_time);
    ResolvePenetration(elapsed_time);
}

float ParticleContact::CalculateSeparatingVelocity() const {
    V3d relative = particles[0]->GetVelocity();
    if (particles[1]) relative += -particles[1]->GetVelocity();
    return relative.dot(normal);
}

void ParticleContact::ResolveVelocity(float elapsed_time) {
    float separate = CalculateSeparatingVelocity();

    if (separate > 0) return; // No impulse required

    float new_separate = -separate * restitution;

    // Check velocity buildup due to acceleration
    V3d acc_velocity = particles[0]->GetAcceleration();
    if (particles[1]) acc_velocity += -particles[1]->GetAcceleration();
    float acc_separate = acc_velocity.dot(normal) * elapsed_time;

    // If closing velocity due to acc buildup, remove it from new separating velocity
    if (acc_separate < 0) {
        new_separate += restitution * acc_separate;
        if (new_separate < 0) new_separate = 0;
    }

    float delta = new_separate - separate;

    float total_inv_mass = particles[0]->GetInverseMass();
    if (particles[1]) total_inv_mass += particles[1]->GetInverseMass();
    if (total_inv_mass <= 0) return; // Both particles have infinite mass, no effect

    float impulse = delta / total_inv_mass;
    V3d impulse_per_unit = normal * impulse; // Impulse per unit of inverse mass

    // Apply impulses
    particles[0]->SetVelocity(particles[0]->GetVelocity() + impulse_per_unit * particles[0]->GetInverseMass());
    if (particles[1]) particles[1]->SetVelocity(particles[1]->GetVelocity() + impulse_per_unit * -particles[1]->GetInverseMass());
}

void ParticleContact::ResolvePenetration(float elapsed_time) {
    if (depth <= 0) return; // no depth

    float total_inv_mass = particles[0]->GetInverseMass();
    if (particles[1]) total_inv_mass += particles[1]->GetInverseMass();
    if (total_inv_mass <= 0) return;

    V3d move_per_unit = normal * (depth / total_inv_mass); // Collision resolution per unit of inverse mass

    // Calculate movement amount
    V3d movement[2];
    movement[0] = move_per_unit * particles[0]->GetInverseMass();
    if (particles[1]) movement[1] = move_per_unit * -particles[1]->GetInverseMass();
    else movement[1] = V3d::origin;

    // Apply collision resolution
    particles[0]->SetPos(particles[0]->GetPos() + movement[0]);
    if (particles[1]) particles[1]->SetPos(particles[1]->GetPos() + movement[1]);
}

ParticleContactResolver::ParticleContactResolver(unsigned iterations): iterations{iterations} {}

void ParticleContactResolver::SetIterations(unsigned iterations) {
    this->iterations = iterations;
}

void ParticleContactResolver::ResolveContacts(ParticleContact *contacts, unsigned count, float elapsed_time) {
    iterations_used = 0;

    while (iterations_used < iterations) {
        float max = __FLT_MAX__;
        unsigned max_index = count;
        // Find contact with largest closing velocity
        for (int i = 0; i < count; ++i) {
            float separate = contacts[i].CalculateSeparatingVelocity(); // Separating velocity

            if (separate < max && (separate < 0 || contacts[i].depth > 0)) {
                max = separate;
                max_index = i;
            }
        }

        if (max_index == count) break;
        else contacts[max_index].Resolve(elapsed_time);

        ++iterations_used;
    }
}
