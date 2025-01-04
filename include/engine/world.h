#pragma once

#include "point.h"
#include "wall.h"

struct World
{
    Point points[4] = {
        Point(-100, 50),
        Point(-50, 100),
        Point(50, 100),
        Point(100, 150),
    };

    Wall walls[3] = {
        Wall(&points[0], &points[1], 0, 70),
        Wall(&points[1], &points[2], 0, 70),
        Wall(&points[2], &points[3], 0, 70),
    };
};
