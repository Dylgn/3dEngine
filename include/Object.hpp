#pragma once

#include "Mesh.hpp"
#include "BoundingBox.hpp"

struct Object {
    Mesh mesh;
    BoundingBox box;

    bool Colliding(Object &o);
};