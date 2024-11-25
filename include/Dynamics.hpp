#pragma once

#include <vector>
#include "Entity.hpp"

void SimulatePhysics(float dt, std::vector<Entity> &entities, int pos_iters, int substeps);