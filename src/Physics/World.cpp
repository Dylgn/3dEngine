#include "World.hpp"

void World::StartFrame() {
    for (Rigidbody *b : bodies) {
        b->ClearAccumulators();
        b->CalculateDerivedData();
    }
}

void World::Run(float elapsed_time) {
    registry.Update(elapsed_time);
}

void World::Integrate(float elapsed_time) {
    for (Rigidbody *b : bodies) b->Integrate(elapsed_time);
}
