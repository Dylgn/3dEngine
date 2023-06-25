#pragma once

#include <string>

struct Texture {
    int width, height;
    unsigned int *image = nullptr;

    bool LoadTexture(std::string file_name);
};