#include <cstdio>
#include <cstring>
#include "Texture.hpp"

Texture::Texture() {}

Texture::Texture(std::string file_name) {
    LoadTexture(file_name);
}

Texture::Texture(const Texture &t): width{t.width}, height{t.height}, image{new uint32_t[width * height]} {
    int img_size = width * height;
    for (int i = 0; i < img_size; ++i) {
        image[i] = t.image[i];
    }
}

Texture::~Texture() {
    delete[] image;
    image = nullptr;
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
    std::fread(&width, sizeof(int32_t), 1, f);
    std::fread(&height, sizeof(int32_t), 1, f);

    bool inverted = (height >= 0); // Image not stored top-down, need to flip
    if (!inverted) height *= -1; // Change to positive height

    // Read image data
    fseek(f, data_offset, SEEK_SET);
    image = new uint32_t[width * height];
    memset(image, 0, sizeof(uint32_t) * width * height);
    std::fread(image, sizeof(uint32_t), width * height, f);

    // Flip image
    if (inverted) {
        for (int i = 0; i < height / 2; ++i) {
            for (int j = 0; j < width; ++j) {
                std::swap(image[i * height + j], image[(height - 1 - i) * height + j]);
            }
        }
    }

    fclose(f);
    return true;
}
