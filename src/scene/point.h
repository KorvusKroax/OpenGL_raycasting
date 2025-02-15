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

    inline Point add(Point p) { return Point(x + p.x, y + p.y); }
    // inline Point add(float x, float y) { return Point(this->x + x, this->y + y); }

    inline Point subtract(Point p) { return Point(x - p.x, y - p.y); }
    // inline Point subtract(float x, float y) { return Point(this->x - x, this->y - y); }

    // inline Point multiply(Point p) { return Point(x * p.x, y * p.y); }
    inline Point multiply(float x, float y) { return Point(this->x * x, this->y * y); }
    inline Point multiply(float v) { return Point(this->x * v, this->y * v); }

    inline Point divide(float v) { return Point(this->x / v, this->y / v); }

    Point rotate(float angle)
    {
        if (angle == 0) return Point(x, y);
        return Point(
            x * cos(angle) - y * sin(angle),
            y * cos(angle) + x * sin(angle)
        );
    }
};
