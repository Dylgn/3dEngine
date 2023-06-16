#include <fstream>
#include <sstream>

#include "V3d.hpp"
#include "Mesh.hpp"

bool Mesh::LoadObject(std::string file_name) {
    std::ifstream f(file_name);
    if (!f.is_open()) return false;

     // Temp storage of vertices
     std::vector<V3d> vertices;

     while (!f.eof()) {
         char line[128];
         f.getline(line, 128);

         std::stringstream s;
         s << line;

         char type;
         if (line[0] == 'v') {
             V3d v;
             s >> type >> v.x >> v.y >> v.z;
             vertices.push_back(v);
         } else if (line[0] == 'f') {
             int f[3];
             s >> type >> f[0] >> f[1] >> f[2];
             triangles.push_back({ vertices[f[0] - 1], vertices[f[1] - 1], vertices[f[2] - 1] });
         }
     }
     
     return true;
}