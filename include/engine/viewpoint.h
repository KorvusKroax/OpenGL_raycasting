#pragma once

#include "point.h"

struct Viewpoint
{
    Point pos;
    float height, heading, pitch;

    Viewpoint(Point pos, float height, float heading, float pitch)
    {
        this->pos = pos;
        this->height = height;
        this->heading = heading;
        this->pitch = pitch;
    }

    void update(float x, float y, float h, float rot_x, float rot_y)
    {
        float dx = sin(heading);
        float dy = cos(heading);
        if (y != 0) this->pos = this->pos.add(dx * y, dy * y);
        if (x != 0) this->pos = this->pos.add(dy * x, -dx * x);

        this->height += h;

        this->heading += rot_x;
        this->pitch += rot_y;
    }
};
