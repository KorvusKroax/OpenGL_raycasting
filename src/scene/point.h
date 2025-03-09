#pragma once

#include <cmath>

struct Point
{
    float x, y;

    Point() { }

    Point(float x, float y)
    {
        this->x = x;
        this->y = y;
    }

    Point(int *serialized_point[])
    {
        this->x = (*serialized_point)[0] / 1000.0f;
        this->y = (*serialized_point)[1] / 1000.0f;
    }

    int serialize(int *serialized_point[])
    {
        int size = 2;
        *serialized_point = new int[size] {
            int(this->x * 1000),
            int(this->y * 1000)
        };

        return size;
    }

    inline Point rotate(float angle)
    {
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
