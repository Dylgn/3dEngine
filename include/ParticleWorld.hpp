#pragma once

#include <vector>
#include "Particle.hpp"
#include "Contact.hpp"
#include "ForceGenerator.hpp"

class ParticleWorld {
    public:
        typedef std::vector<Particle*> Particles;
        typedef std::vector<ParticleContactGenerator*> ContactGenerators;
    protected:
        Particles particles; // Particles in world

        bool calculate_iterations; // If world should calculate number of iterations to give contact resolver at each frame

        ForceRegistry registry; // Force registers for particles in world

        ParticleContactResolver resolver; // Resolver for contacts in world

        ContactGenerators c_gens; // Contact generators

        ParticleContact *contacts; // List of contacts

        unsigned max_contacts; // Max number of contacts allowed (size of contacts array)
    public:
        /** Creates a physics simulation.
         * @param max_contacts Max number of contacts to be handled per frame
         * @param iterations Number of contact-resolutions to use. Defaults to twice the number of contacts
        */
        ParticleWorld(unsigned max_contacts, unsigned iterations = 0);

        ~ParticleWorld();

        /** Initializes for simulation frame. Clears force accumulators. After method is called, particles can have their forces applied. */
        void StartFrame();

        /** Calls each registered contact generator to report their contacts.
         * @return Number of generated contacts
        */
        unsigned GenerateContacts();

        /** Integrates all particles forward in time. */
        void Integrate(float elapsed_time);

        /** Processes physics for world. */
        void Update(float elapsed_time);
};