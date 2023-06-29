#pragma once

#include <string>

struct Texture {
    int32_t width = 0, height = 0;
    uint32_t *image = nullptr;

    Texture();
    Texture(std::string file_name);
    Texture(Texture &t);
    ~Texture();

    /** Load bmp texture file. Assumes it is 32-bit rgb */
    bool LoadTexture(std::string file_name);
};