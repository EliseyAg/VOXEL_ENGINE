#include "png_loader.hpp"

#include <iostream>
#include <glad/glad.h>
#include "../Graphics/Texture.hpp"

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG
#include "stb_image.h"

int _png_load(const char* file, int* width, int* height) {
    int channels = 0;
    GLuint alpha;
    GLuint texture;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* pixels = stbi_load(std::string(file).c_str(), width, height, &channels, 0);
    if (!pixels)
    {
        std::cerr << "Can't load image: " << file << std::endl << stbi_failure_reason() << std::endl;
        return -1;
    }

    switch (channels) {
    case 4:
        alpha = GL_RGBA;
        break;
    case 3:
        alpha = GL_RGB;
        break;
    default:
        alpha = GL_RGBA;
        return 0;
    }
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, alpha, *width, *height, 0, alpha, GL_UNSIGNED_BYTE, pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(pixels);
    return texture;
}

Texture* load_texture(std::string filename) {
    int width, height;
    GLuint texture = _png_load(filename.c_str(), &width, &height);
    if (texture == 0) {
        std::cerr << "Could not load texture " << filename << std::endl;
        return nullptr;
    }
    return new Texture(texture, width, height);
}