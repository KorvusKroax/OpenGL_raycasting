#include <algorithm> // std::min, std::max

#include "canvas.h"

void Canvas::drawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, Color color)
{
    drawLine(x1, y1, x2, y2, color);
    drawLine(x2, y2, x3, y3, color);
    drawLine(x3, y3, x1, y1, color);
}

void Canvas::drawTriangle_filled(int x1, int y1, int x2, int y2, int x3, int y3, Color color)
{
    int xMin = std::min({x1, x2, x3});
    int yMin = std::min({y1, y2, y3});
    int xMax = std::max({x1, x2, x3});
    int yMax = std::max({y1, y2, y3});

    if ((xMax < 0) || (xMin >= width) || (yMax < 0) || (yMin >= height)) {
        return;
    }

    for (int x = xMin; x < xMax; x++) {
        for (int y = yMin; y < yMax; y++) {
            if (cross(x1, y1, x2, y2, x, y) <= 0 &&
                cross(x2, y2, x3, y3, x, y) <= 0 &&
                cross(x3, y3, x1, y1, x, y) <= 0) {
                    setPixel(x, y, color);
            }
        }
    }
}

void Canvas::drawTriangle_filled_withBias(int x1, int y1, int x2, int y2, int x3, int y3, Color color)
{
    int xMin = std::min({x1, x2, x3});
    int yMin = std::min({y1, y2, y3});
    int xMax = std::max({x1, x2, x3});
    int yMax = std::max({y1, y2, y3});

    if ((xMax < 0) || (xMin >= width) || (yMax < 0) || (yMin >= height)) {
        return;
    }

    int bias1 = (y1 == y2 && x1 < x2) || y1 < y2 ? 0 : 1;
    int bias2 = (y2 == y3 && x2 < x3) || y2 < y3 ? 0 : 1;
    int bias3 = (y3 == y1 && x3 < x1) || y3 < y1 ? 0 : 1;

    for (int x = xMin; x < xMax; x++) {
        for (int y = yMin; y < yMax; y++) {
            if (cross(x1, y1, x2, y2, x, y) + bias1 <= 0 &&
                cross(x2, y2, x3, y3, x, y) + bias2 <= 0 &&
                cross(x3, y3, x1, y1, x, y) + bias3 <= 0) {
                    setPixel(x, y, color);
            }
        }
    }
}

inline int Canvas::cross(int x1, int y1, int x2, int y2, int px, int py)
{
    return (x2 - x1) * (py - y1) - (y2 - y1) * (px - x1);
    // n > 0: p is left
    // n = 0: p is on line
    // n < 0: p is right
}
