#pragma once

#include "point.h"

struct Viewpoint
{
    Point pos;
    float height, heading;

    Viewpoint(Point pos, float height, float heading)
    {
        this->pos = pos;
        this->height = height;
        this->heading = heading;
    }

    void update(float x, float y, float rot)
    {
        float dx = sin(heading);
        float dy = cos(heading);
        if (y != 0) this->pos = this->pos.add(dx * y, dy * y);
        if (x != 0) this->pos = this->pos.add(dy * x, -dx * x);

        this->heading += rot;
    }
};
