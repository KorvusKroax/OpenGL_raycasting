#pragma once

#include <iostream>

// #define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

struct Texture
{
    int width, height, channelCount;
    int *pixels;

    Texture() { }

    Texture(const char *fileName)
    {
        stbi_set_flip_vertically_on_load(true);
        unsigned char *image = stbi_load(fileName, &this->width, &this->height, &this->channelCount, 0);

        this->pixels = new int[this->width * this->height];
        for (int i = 0; i < this->width; i++) {
            for (int j = 0; j < this->height; j++) {
                this->pixels[i + j * this->width] =
                    image[(i * this->channelCount + 0) + j * (this->width * this->channelCount)] |
                    image[(i * this->channelCount + 1) + j * (this->width * this->channelCount)] << 8 |
                    image[(i * this->channelCount + 2) + j * (this->width * this->channelCount)] << 16 |
                    255 << 24;
            }
        }

        stbi_image_free(image);
    }

    Texture(int* serialized_texture[])
    {
        this->width = (*serialized_texture)[0];
        this->height = (*serialized_texture)[1];
        this->channelCount = (*serialized_texture)[2];
        this->pixels = new int[this->width * this->height];
        for (int i = 0; i < this->width * this->height; i++) {
            this->pixels[i] = (*serialized_texture)[3 + i];
        }
    }

    int serialize(int *serialized_texture[])
    {
        int size = 3 + this->width * this->height;
        *serialized_texture = new int[size];

        (*serialized_texture)[0] = this->width;
        (*serialized_texture)[1] = this->height;
        (*serialized_texture)[2] = this->channelCount;
        for (int i = 0; i < this->width * this->height; i++) {
            (*serialized_texture)[3 + i] = this->pixels[i];
        }

        return size;
    }
};
