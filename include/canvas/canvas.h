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

        void drawLine(int x1, int y1, int x2, int y2, Color color);
        void drawLine_AA(int x1, int y1, int x2, int y2, Color color);

        void drawBezierCurve_quadratic(int x1, int y1, int xc, int yc, int x2, int y2, float resolution, Color color);
        void drawBezierCurve_cubic(Canvas* canvas, int x1, int y1, int x1c, int y1c, int x2c, int y2c, int x2, int y2, float resolution, Color color);

        void drawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, Color color);
        void drawTriangle_filled(int x1, int y1, int x2, int y2, int x3, int y3, Color color);
        void drawTriangle_filled_withBias(int x1, int y1, int x2, int y2, int x3, int y3, Color color);
        int cross(int x1, int y1, int x2, int y2, int px, int py);

        void drawRectangle(int x, int y, int w, int h, Color color);
        void drawRectangle_filled(int x, int y, int w, int h, Color color);

        void drawCircle(int cx, int cy, int r, Color color);
        void drawCircle_midpoint(int cx, int cy, int r, Color color);
        void drawCircle_filled(int cx, int cy, int r, Color color);

        void floodFill(int x, int y, Color color);
        void spanFill(int x, int y, Color color);
};
