#pragma once

#include <canvas/color.h>

#include <cstring>

struct Canvas
{
    public:
        unsigned int width;
        unsigned int height;
        int* pixels;

        Canvas() { }

        ~Canvas()
        {
            delete[] pixels;
        }

        Canvas(unsigned int width, unsigned int height)
        {
            init(width, height);
        }

        void init(unsigned int width, unsigned int height)
        {
            this->width = width;
            this->height = height;
            this->pixels = new int[this->width * this->height];
        }

        void clearCanvas()
        {
            memset(pixels, 0, width * height * sizeof(int));
        }

        void fillCanvas(Color color)
        {
            for (int i = 0; i < width * height; i++) pixels[i] = color.value;
        }

        void setPixel(int x, int y, Color color)
        {
            if (x < 0 || x >= width || y < 0 || y >= height) return;

            if (color.getAlpha() == 255) {
                pixels[x + y * width] = color.value;
                return;
            }

            if (color.getAlpha() == 0) {
                return;
            }

            Color currPixel = Color(pixels[x + y * width]);
            float weight = color.getAlpha() / 255.0f;
            int r = int(currPixel.getRed()   * (1.0f - weight) + color.getRed()   * weight);
            int g = int(currPixel.getGreen() * (1.0f - weight) + color.getGreen() * weight);
            int b = int(currPixel.getBlue()  * (1.0f - weight) + color.getBlue()  * weight);
            int a = (currPixel.getAlpha() + color.getAlpha()) >> 1;

            pixels[x + y * width] = Color(r, g, b, a).value;
        }

        bool getPixel(int x, int y, Color* color = 0)
        {
            if (x < 0 || x >= width || y < 0 || y >= height) return false;

            *color = Color(pixels[x + y * width]);
            return true;
        }

        void setPixels(int x, int y, Canvas* canvas)
        {
            for (int i = 0; i < canvas->width; i++) {
                for (int j = 0; j < canvas->height; j++) {
                    setPixel(x + i, y + j, canvas->pixels[i + j * canvas->width]);
                }
            }
        }

        bool getPixels(int x, int y, unsigned int w, unsigned int h, Canvas* canvas)
        {
            canvas = new Canvas(w, h);

            if ((x < 0 && x + canvas->width < 0) || (x >= width && x + canvas->width > width) ||
                (y < 0 && y + canvas->height < 0) || (y >= height && x + canvas->height > height)) return false;

            Color color;
            for (int i = 0; i < w; i++) {
                for (int j = 0; j < h; j++) {
                    getPixel(x + i, y + j, &color);
                    canvas->pixels[i + j * w] = color.value;
                }
            }

            return true;
        }
};

#include <canvas/line.h>
#include <canvas/bezier_curve.h>
#include <canvas/triangle.h>
#include <canvas/rectangle.h>
#include <canvas/circle.h>
#include <canvas/fill.h>
