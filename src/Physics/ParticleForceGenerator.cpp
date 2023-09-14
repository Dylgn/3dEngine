#include "ParticleForceGenerator.hpp"

void ParticleForceRegistry::Add(Particle *p, ParticleForceGenerator *pfg) {
    registrations.push_back({p, pfg});
}

void ParticleForceRegistry::Remove(Particle *p, ParticleForceGenerator *pfg) {
    for (Registry::iterator it = registrations.begin(); it != registrations.end(); ++it) {
        if (it->p == p && it->pfg == pfg) {
            registrations.erase(it);
            return;
        }
    }
}

void ParticleForceRegistry::Clear() {
    registrations.clear();
}

void ParticleForceRegistry::Update(float elapsed_time) {
    for (ForceRegistration pfr : registrations) {
        pfr.pfg->UpdateForce(pfr.p, elapsed_time);
    }
}
