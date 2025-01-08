#pragma once

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

struct Texture
{
    int width, height, channelCount;
    int* pixels;

    Texture() { }

    Texture(const char* fileName) //{ loadTexture(fileName); }
    {
        stbi_set_flip_vertically_on_load(true);
        unsigned char *image = stbi_load(fileName, &width, &height, &channelCount, 0);

        pixels = new int[width * height];
        for (int i = 0; i < width; i++) {
            for (int j = 0; j < height; j++) {
                pixels[i + j * width] =
                    image[(i * channelCount + 0) + j * width * channelCount] |
                    image[(i * channelCount + 1) + j * width * channelCount] << 8 |
                    image[(i * channelCount + 2) + j * width * channelCount] << 16 |
                    255 << 24;
            }
        }

        stbi_image_free(image);
    }

    ~Texture() { delete[] pixels; }

    // void loadTexture(const char* fileName)
    // {
    //     int width, height, channelCount;

    //     stbi_set_flip_vertically_on_load(true);
    //     unsigned char *image = stbi_load(fileName, &width, &height, &channelCount, 0);

    //     pixels = new int[width * height];
    //     for (int i = 0; i < width; i++) {
    //         for (int j = 0; j < height; j++) {
    //             pixels[i + j * width] =
    //                 image[(i * channelCount + 0) + j * width * channelCount] |
    //                 image[(i * channelCount + 1) + j * width * channelCount] << 8 |
    //                 image[(i * channelCount + 2) + j * width * channelCount] << 16 |
    //                 255 << 24;
    //         }
    //     }

    //     stbi_image_free(image);
    // }
};
