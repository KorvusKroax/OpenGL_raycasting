#include "canvas.h"

void Canvas::drawRectangle(int x, int y, int w, int h, Color color)
{
    for (int i = 0; i < w; i++) {
        setPixel(x + i, y, color);
        setPixel(x + i, y + h - 1, color);
    }
    for (int i = 1; i < h - 1; i++) {
        setPixel(x, y + i, color);
        setPixel(x + w - 1, y + i, color);
    }
}

void Canvas::drawRectangle_filled(int x, int y, int w, int h, Color color)
{
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++)  {
            setPixel(x + j, y + i, color);
        }
    }
}
