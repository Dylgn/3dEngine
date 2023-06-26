#pragma once

#include <string>

struct Texture {
    int32_t width, height;
    uint32_t *image = nullptr;

    bool LoadTexture(std::string file_name); // Assumes 32-bit rgb bmp file
};