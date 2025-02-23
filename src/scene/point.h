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

    Point rotate(float angle)
    {
        if (angle == 0) return Point(this->x, this->y);
        return Point(
            this->x * cos(angle) - this->y * sin(angle),
            this->y * cos(angle) + this->x * sin(angle)
        );
    }

    Point operator + (Point p) { return Point(this->x + p.x, this->y + p.y); }
    Point operator - (Point p) { return Point(this->x - p.x, this->y - p.y); }
    Point operator * (Point p) { return Point(this->x * p.x, this->y * p.y); }
    Point operator * (float v) { return Point(this->x * v, this->y * v); }
    Point operator / (float v) { return Point(this->x / v, this->y / v); }
};
