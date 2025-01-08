#pragma once

#include "point.h"

struct Wall
{
    Point* start;
    Point* end;
    float bottom, top;
    Color color;
    int texture_id;

    Wall(Point* start, Point* end, float bottom, float top, Color color, int texture_id)
    {
        this->start = start;
        this->end = end;
        this->bottom = bottom;
        this->top = top;
        this->color = color;
        this->texture_id = texture_id;
    }
};
