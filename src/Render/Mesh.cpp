#include <fstream>
#include <sstream>

#include "V3d.hpp"
#include "Mesh.hpp"

Mesh::Mesh() {}

Mesh::Mesh(std::string file_name, bool textured) {
    LoadObject(file_name, textured);
}

std::vector<V3d> Mesh::LoadObject(std::string file_name, bool textured) {
    std::vector<V3d> vertices;
    std::ifstream f(file_name);
    if (!f.is_open()) return vertices;

     // Temp storage of texture coords
     std::vector<V2d> textures;

     while (!f.eof()) {
         char line[128];
         f.getline(line, 128);

         std::stringstream s;
         s << line;

         char type;

         switch (line[0]) {
            case 'v':
                if (line[1] == 't') { // Texture
                    V2d v;
                    s >> type >> type >> v.u >> v.v;
                    textures.push_back(v);
                } else {
                    V3d v;
                    s >> type >> v.x >> v.y >> v.z;
                    vertices.push_back(v);
                }
                break;
            case 'f':
                if (textured) {
                    s >> type;

                    std::string tokens[6];
                    int token_count = -1;

                    while (!s.eof()) {
                        char c = s.get();
                        
                        if (c == ' ' || c == '/') ++token_count;
                        else tokens[token_count].append(1, c);
                    }

                    tokens[token_count].pop_back();

                    m_triangles.push_back(
                        { vertices[stoi(tokens[0]) - 1], vertices[stoi(tokens[2]) - 1], vertices[stoi(tokens[4]) - 1],
                          textures[stoi(tokens[1]) - 1], textures[stoi(tokens[3]) - 1], textures[stoi(tokens[5]) - 1] }
                    );
                } else {
                    int f[3];
                    s >> type >> f[0] >> f[1] >> f[2];
                    m_triangles.push_back({ vertices[f[0] - 1], vertices[f[1] - 1], vertices[f[2] - 1] });
                }
                break;
            default:
                break;
         }
     }
     return vertices;
}

void Mesh::Move(const V3d &dir) {
    for (auto &t : m_triangles) {
        t.p[0] += dir;
        t.p[1] += dir;
        t.p[2] += dir;
    }
}

V3d Mesh::GetCenter() const {
    V3d avg;
    for (auto &t : m_triangles) {
        avg += t.p[0];
        avg += t.p[1];
        avg += t.p[2];
    }
    float count = 3 * m_triangles.size();
    return avg / count;
}
