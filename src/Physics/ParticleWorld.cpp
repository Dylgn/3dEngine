#include "ParticleWorld.hpp"

ParticleWorld::ParticleWorld(unsigned max_contacts, unsigned iterations): 
    calculate_iterations{iterations == 0}, resolver{iterations}, contacts{new ParticleContact[max_contacts]}, max_contacts{max_contacts} {}

ParticleWorld::~ParticleWorld() {
    delete[] contacts;
}

unsigned ParticleWorld::GenerateContacts()
{
    unsigned limit = max_contacts;
    ParticleContact *next_contact = contacts;

    for (auto c_gen : c_gens) {
        unsigned used = c_gen->AddContact(next_contact, limit);
        limit -= used;
        next_contact += used;

        if (limit <= 0) break;
    }

    return max_contacts - limit;
}

void ParticleWorld::Integrate(float elapsed_time) {
    for (auto p : particles) p->Integrate(elapsed_time);
}

void ParticleWorld::Update(float elapsed_time) {
    registry.Update(elapsed_time); // Apply force generators

    Integrate(elapsed_time); // Integrate objects

    unsigned used_contacts = GenerateContacts(); // Process contacts
    if (used_contacts) {
        if (calculate_iterations) resolver.SetIterations(used_contacts * 2);
        resolver.ResolveContacts(contacts, used_contacts, elapsed_time);
    }

}
