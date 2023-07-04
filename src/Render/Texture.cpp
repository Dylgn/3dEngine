#include <cstdio>
#include <cstring>
#include "Texture.hpp"

Texture::Texture() {}

Texture::Texture(std::string file_name) {
    LoadTexture(file_name);
}

Texture::Texture(const Texture &t): m_width{t.m_width}, m_height{t.m_height}, m_image{new uint32_t[m_width * m_height]} {
    int img_size = m_width * m_height;
    for (int i = 0; i < img_size; ++i) {
        m_image[i] = t.m_image[i];
    }
}

Texture::~Texture() {
    delete[] m_image;
    m_image = nullptr;
}

bool Texture::LoadTexture(std::string file_name) {
    // Opens file and checks if file is opened
    FILE* f = fopen(file_name.c_str(), "rb");
    if (!f) return false;

    // Checks if file is bmp
    int16_t isBMP;
    std::fread(&isBMP, sizeof(int16_t), 1, f);
    if (isBMP != 0x4D42) {
        fclose(f);
        return false;
    }

    // Get offset to start of image data
    int32_t data_offset;

    fseek(f, 0x08, SEEK_CUR);
    std::fread(&data_offset, sizeof(int32_t), 1, f);

    // Get image dimensions and size
    fseek(f, 0x04, SEEK_CUR);
    std::fread(&m_width, sizeof(int32_t), 1, f);
    std::fread(&m_height, sizeof(int32_t), 1, f);

    bool inverted = (m_height >= 0); // Image not stored top-down, need to flip
    if (!inverted) m_height *= -1; // Change to positive height

    // Read image data
    fseek(f, data_offset, SEEK_SET);
    m_image = new uint32_t[m_width * m_height];
    memset(m_image, 0, sizeof(uint32_t) * m_width * m_height);
    std::fread(m_image, sizeof(uint32_t), m_width * m_height, f);

    // Flip image
    if (inverted) {
        for (int i = 0; i < m_height / 2; ++i) {
            for (int j = 0; j < m_width; ++j) {
                std::swap(m_image[i * m_width + j], m_image[(m_height - 1 - i) * m_width + j]);
            }
        }
    }

    fclose(f);
    return true;
}
