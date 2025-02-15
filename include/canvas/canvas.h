#pragma once

#include <cstring>

#include <color.h>

struct Canvas
{
    public:
        unsigned int width, height;
        int* pixels;

        Canvas();
        ~Canvas();
        Canvas(unsigned int width, unsigned int height);

        void init(unsigned int width, unsigned int height);
        void clearCanvas();
        void fillCanvas(Color color);
        void setPixel(int x, int y, Color color);
        bool getPixel(int x, int y, Color* color = 0);
        void setPixels(int x, int y, Canvas* canvas);
        bool getPixels(int x, int y, unsigned int w, unsigned int h, Canvas* canvas);
};
