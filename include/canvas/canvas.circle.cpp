#include <canvas.h>

void Canvas::drawCircle(int cx, int cy, int r, Color color)
{
    int x = 0;
    int y = -r;
    int p = 2 - 2 * r;

    while (y < 0) {
        setPixel(cx - x, cy + y, color);
        setPixel(cx - y, cy - x, color);
        setPixel(cx + x, cy - y, color);
        setPixel(cx + y, cy + x, color);
        r = p;
        if (r <= x) p += ++x * 2 + 1;
        if (r > y || p > x) p += ++y * 2 + 1;
    }
}

void Canvas::drawCircle_midpoint(int cx, int cy, int r, Color color)
{
    int x = 0;
    int y = -r;
    int p = -r;

    while (x < -y) {
        if (p > 0) p += 2 * (x + ++y) + 1;
        else p += 2 * x + 1;
        setPixel(cx + x, cy + y, color);
        setPixel(cx - x, cy + y, color);
        setPixel(cx + x, cy - y, color);
        setPixel(cx - x, cy - y, color);
        setPixel(cx + y, cy + x, color);
        setPixel(cx - y, cy + x, color);
        setPixel(cx + y, cy - x, color);
        setPixel(cx - y, cy - x, color);
        x++;
    }
}

void Canvas::drawCircle_filled(int cx, int cy, int r, Color color)
{
    int rr = r;
    int x = -r;
    int y = 0;
    int p = 2 - 2 * r;

    for (int i = cx + x; i <= cx - x; i++) setPixel(i, cy + y, color);
    while (y < rr) {
        r = p;
        if (r > x || p > y) p += ++x * 2 + 1;
        if (r > y) continue;
        p += ++y * 2 + 1;
        for (int i = cx + x; i <= cx - x; i++) setPixel(i, cy + y, color);
        for (int i = cx + x; i <= cx - x; i++) setPixel(i, cy - y, color);
    }
}
