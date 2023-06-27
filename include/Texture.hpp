#pragma once

#include <string>

struct Texture {
    int32_t width = 0, height = 0;
    uint32_t *image = nullptr;

    Texture();
    Texture(std::string file_name);
    Texture(Texture &t);
    ~Texture();

    bool LoadTexture(std::string file_name); // Assumes 32-bit rgb bmp file
};