#include "ForceGenerator.hpp"

void ForceRegistry::Add(Particle *p, ForceGenerator *pfg) {
    registrations.push_back({p, pfg});
}

void ForceRegistry::Remove(Particle *p, ForceGenerator *pfg) {
    for (Registry::iterator it = registrations.begin(); it != registrations.end(); ++it) {
        if (it->p == p && it->pfg == pfg) {
            registrations.erase(it);
            return;
        }
    }
}

void ForceRegistry::Clear() {
    registrations.clear();
}

void ForceRegistry::Update(float elapsed_time) {
    for (ForceRegistration pfr : registrations) {
        pfr.pfg->UpdateForce(pfr.p, elapsed_time);
    }
}
