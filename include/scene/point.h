#pragma once

#include <math.h>

struct Point
{
    float x, y;

    Point(float x = 0, float y = 0)
    {
        this->x = x;
        this->y = y;
    }

    // Point add(Point p) { return Point(x + p.x, y + p.y); }
    // Point add(float x, float y) { return Point(this->x + x, this->y + y); }

    // Point sub(Point p) { return Point(x - p.x, y - p.y); }
    // Point sub(float x, float y) { return Point(this->x - x, this->y - y); }

    // Point mult(Point p) { return Point(x * p.x, y * p.y); }
    // Point mult(float x, float y) { return Point(this->x * x, this->y * y); }
    // Point mult(float v) { return Point(this->x * v, this->y * v); }

    // Point rotate(float angle)
    // {
    //     if (angle == 0) return Point(x, y);
    //     return Point(
    //         x * cos(angle) - y * sin(angle),
    //         y * cos(angle) + x * sin(angle)
    //     );
    // }
};
