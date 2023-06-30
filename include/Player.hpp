#pragma once

#include "Camera.hpp"
#include "BoundingBox.hpp"

struct Player {
    Camera cam;
    BoundingBox box;
};