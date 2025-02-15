#pragma once

#include <canvas.h>
#include <color.h>

namespace Rectangle
{
    void draw(Canvas* canvas, int x, int y, int w, int h, Color color)
    {
        for (int i = 0; i < w; i++) {
            canvas->setPixel(x + i, y        , color);
            canvas->setPixel(x + i, y + h - 1, color);
        }
        for (int i = 1; i < h - 1; i++) {
            canvas->setPixel(x        , y + i, color);
            canvas->setPixel(x + w - 1, y + i, color);
        }
    }

    void draw_filled(Canvas* canvas, int x, int y, int w, int h, Color color)
    {
        for (int i = 0; i < h; i++) {
            for (int j = 0; j < w; j++)  {
                canvas->setPixel(x + j, y + i, color);
            }
        }
    }
}
