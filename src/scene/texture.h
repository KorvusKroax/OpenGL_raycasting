#pragma once

#include <iostream>

#include "lodepng/lodepng.h"

struct Texture
{
    int width, height;
    int *pixels;

    Texture() { }

    Texture(const char *fileName)
    {
        unsigned imageWidth, imageHeight, channelCount = 4;
        unsigned char *image;
        unsigned error = lodepng_decode32_file(&image, &imageWidth, &imageHeight, fileName);
        if (error) {
            std::cout << "decoder error " << error << ": " << lodepng_error_text(error) << std::endl;
        }

        this->width = imageWidth;
        this->height = imageHeight;
        this->pixels = new int[this->width * this->height];
        for (int i = 0; i < this->width; i++) {
            for (int j = 0; j < this->height; j++) {
                this->pixels[i + j * this->width] =
                    image[(i * channelCount + 0) + (this->height - 1 - j) * (this->width * channelCount)] |
                    image[(i * channelCount + 1) + (this->height - 1 - j) * (this->width * channelCount)] << 8 |
                    image[(i * channelCount + 2) + (this->height - 1 - j) * (this->width * channelCount)] << 16 |
                    image[(i * channelCount + 3) + (this->height - 1 - j) * (this->width * channelCount)] << 24;
            }
        }
    }

    Texture(int* serialized_texture[])
    {
        this->width = (*serialized_texture)[0];
        this->height = (*serialized_texture)[1];
        this->pixels = new int[this->width * this->height];
        for (int i = 0; i < this->width * this->height; i++) {
            this->pixels[i] = (*serialized_texture)[2 + i];
        }
    }

    int serialize(int *serialized_texture[])
    {
        int size = 2 + this->width * this->height;
        *serialized_texture = new int[size];

        (*serialized_texture)[0] = this->width;
        (*serialized_texture)[1] = this->height;
        for (int i = 0; i < this->width * this->height; i++) {
            (*serialized_texture)[2 + i] = this->pixels[i];
        }

        return size;
    }
};
