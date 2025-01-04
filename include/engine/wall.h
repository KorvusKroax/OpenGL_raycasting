#pragma once

#include "point.h"

struct Wall
{
    Point* start;
    Point* end;
    float bottom, top;

    Wall(Point* start, Point* end, float bottom, float top)
    {
        this->start = start;
        this->end = end;
        this->bottom = bottom;
        this->top = top;
    }
};
