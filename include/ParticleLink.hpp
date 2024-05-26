#pragma once

#include "ParticleContact.hpp"

struct ParticleLink: public ParticleContactGenerator {
    Particle *particles[2]; // Pair of particles connected by link

    virtual unsigned AddContact(Particle *contact, unsigned limit) const = 0;

    protected:
        float Length() const; // Current length of link
};

/** Links 2 particles. Generates contact if max length is exceeded. */
struct ParticleCable: public ParticleLink {
    float max_length; // Max length of cable

    float restitution; // Restitution (bounciness) of cable

    /** Fills given contact structure with contact needed to keep cable from overextending. */
    virtual unsigned AddContact(ParticleContact *contact, unsigned limit) const;
};

/** Links 2 particles. Generates contact if not kept at given length */
struct ParticleRod: public ParticleLink {
    float length; // Length of rod

    /** Fills given contact structure with contact needed to keep rod from extending/compressing. */
    virtual unsigned AddContact(ParticleContact *contact, unsigned limit) const;
};