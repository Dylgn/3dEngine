#pragma once

#include <string>

struct Texture {
    int32_t m_width = 0, m_height = 0;
    uint32_t *m_image = nullptr;

    Texture();
    Texture(std::string file_name);
    Texture(const Texture &t);
    ~Texture();

    /** Load bmp texture file. Assumes it is 32-bit rgb */
    bool LoadTexture(std::string file_name);
};