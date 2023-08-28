#include "BuoyancyGenerator.hpp"

BuoyancyGenerator::BuoyancyGenerator(float max_depth, float volume, float liquid_height, float liquid_density):
    max_depth{max_depth}, volume{volume}, liquid_height{liquid_height}, liquid_density{liquid_density} {}

void BuoyancyGenerator::UpdateForce(Particle *p, float elapsed_time) {
    float depth = p->GetPos().y;

    V3d force = V3d::origin;
    if (depth >= liquid_height + max_depth) return; // Check if out of water
    else if (depth <= liquid_height - max_depth) { // Check if max depth
        force.y = liquid_density * volume;
        p->ApplyForce(force);
    } else { // Partially submerged
        force.y = liquid_density * volume * (depth - max_depth - liquid_height) / 2 * max_depth;
        p->ApplyForce(force);
    }
}
