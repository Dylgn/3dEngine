#pragma once

#include <vector>
#include <list>

#include "Mesh.hpp"
#include "M4x4.hpp"
#include "V3d.hpp"
#include "Triangle.hpp"
#include "Camera.hpp"

std::vector<Triangle> GetTriangles(Mesh mesh, Camera &cam, int width, int height);
std::list<Triangle> ClipTriangles(std::vector<Triangle> triangles, int width, int height);
std::list<Triangle> GetClippedTriangles(Mesh mesh, Camera &cam, float width, float height);
std::vector<Triangle> SortDepth(std::vector<Triangle> triangles);