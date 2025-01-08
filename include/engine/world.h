#pragma once

#include "point.h"
#include "wall.h"
#include "texture.h"

struct World
{
    static const int point_count = 4;
    Point points[point_count] = {
        Point(-150, 0),
        Point(-64, 100),
        Point(64, 100),
        Point(150, 300),
    };

    static const int wall_count = 3;
    Wall walls[wall_count] = {
        Wall(&points[0], &points[1], 0, 150, Color(0, 100, 100), 1),
        Wall(&points[1], &points[2], 0, 150, Color(100, 100, 0), 0),
        Wall(&points[2], &points[3], 0, 150, Color(100, 0, 100), 2),
    };

    static const int texture_count = 3;
    Texture textures[texture_count] = {
        Texture("textures/161.png"),
        Texture("textures/211.png"),
        Texture("textures/212.png"),
    };
};
