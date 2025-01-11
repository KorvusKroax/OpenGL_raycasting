#pragma once

#include "point.h"
#include "texture.h"
#include "canvas/color.h"

struct Wall
{
    Point* start;
    Point* end;
    Color color;
    Texture* texture;
    int portal_to;

    Wall(Point* start, Point* end, Color color, Texture* texture, int portal_to = -1)
    {
        this->start = start;
        this->end = end;
        this->color = color;
        this->texture = texture;
        this->portal_to = portal_to;
    }
};
