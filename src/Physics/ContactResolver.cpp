#include "ContactResolver.hpp"

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
