#pragma once

#include <vector>
#include <string>

#include "Triangle.hpp"

struct Mesh {
    std::vector<Triangle> triangles;

    bool LoadObject(std::string file_name, bool textured = false);
};