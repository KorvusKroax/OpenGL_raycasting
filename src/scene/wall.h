#pragma once

#include <canvas/color.h>
#include "point.h"
#include "texture.h"

struct Wall
{
    Point* start;
    Point* end;
    Texture* texture;
    int portalTo; // index of a sector

    Wall(Point* start, Point* end, Texture* texture, int portalTo = -1)
    {
        this->start = start;
        this->end = end;
        this->texture = texture;
        this->portalTo = portalTo;
    }

    bool backfaceCulling(Point p)
    {
        float x1 = start->x - p.x;
        float y1 = start->y - p.y;
        float x2 = end->x - p.x;
        float y2 = end->y - p.y;
        return (y2 - y1) * x1 - (x2 - x1) * y1 >= 0;
    }
};
