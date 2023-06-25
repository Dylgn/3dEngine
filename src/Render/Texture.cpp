#include <cstdio>
#include "Texture.hpp"

bool Texture::LoadTexture(std::string file_name) {
    FILE* f = fopen(file_name.c_str(), "rb");
    
    return true;
}