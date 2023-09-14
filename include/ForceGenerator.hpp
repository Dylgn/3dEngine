#pragma once

#include <vector>
#include "Rigidbody.hpp"

struct ForceGenerator {
    virtual void UpdateForce(Rigidbody *b, float elapsed_time) = 0;
};

class ForceRegistry {
    protected:
        struct ForceRegistration {
            Rigidbody *b;
            ForceGenerator *fg;
        };

        typedef std::vector<ForceRegistration> Registry;
        Registry registrations;
    public:
        /** Registers given particle and force generator pair. */
        void Add(Rigidbody *b, ForceGenerator *fg);

        /** Removes given particle and force generator pair. No effect if pair is not registered */
        void Remove(Rigidbody *b, ForceGenerator *fg);

        /** Clears all registrations from registry. */
        void Clear();

        /** Update particles in registry with their corresponding force generators. */
        void Update(float elapsed_time);
};