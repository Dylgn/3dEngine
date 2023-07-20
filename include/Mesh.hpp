#pragma once

#include <vector>
#include <string>

#include "Triangle.hpp"

struct Mesh {
    std::vector<Triangle> m_triangles;

    Mesh();
    Mesh(std::string file_name, bool textured = false);
    
    /** Load .obj file
     * @return Vector of vertices of the object
    */
    std::vector<V3d> LoadObject(std::string file_name, bool textured = false);
    /** Move mesh in given direction */
    void Move(const V3d &dir);
    /** Gets center of collider */
    V3d GetCenter() const;
};