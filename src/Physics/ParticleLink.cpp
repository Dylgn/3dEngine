#include "ParticleLink.hpp"

float ParticleLink::Length() const {
    V3d relative = particles[0]->GetPos() - particles[1]->GetPos();
    return relative.length();
}

unsigned ParticleCable::AddContact(ParticleContact *contact, unsigned limit) const {
    // Return if overextended
    float length = Length();
    if (length < max_length) return 0;

    // Otherwise, return contact
    contact->particles[0] = particles[0];
    contact->particles[1] = particles[1];

    // Calculate normal
    V3d normal = particles[1]->GetPos() - particles[0]->GetPos();
    normal = normal.normalize();
    contact->normal = normal;

    contact->depth = length - max_length;
    contact->restitution = restitution;

    return 1;
}

unsigned ParticleRod::AddContact(ParticleContact *contact, unsigned limit) const {
    // Return if at regular length
    float cur_length = Length();
    if (cur_length == length) return 0;
    
    // Otherwise, return the contact
    contact->particles[0] = particles[0];
    contact->particles[1] = particles[1];

    // Calculate normal
    V3d normal = particles[1]->GetPos() - particles[0]->GetPos();
    normal = normal.normalize();

    if (cur_length > length) {
        contact->normal = normal;
        contact->depth = cur_length - length;
    } else {
        contact->normal = -normal;
        contact->depth = length - cur_length;
    }

    // Always no restitution (no bounciness)
    contact->restitution = 0;
    
    return 1;
}
