#pragma once

#include "point.h"
#include "wall.h"
#include "sector.h"
#include "texture.h"

struct World
{
    static const int point_count = 8;
    Point points[point_count] = {
        Point(-64, 200),
        Point(64, 200),

        Point(200, 50),
        Point(0, -200),
        Point(-200, -50),

        Point(200, 250),
        Point(0, 500),
        Point(-200, 300),
    };

    static const int texture_count = 3;
    Texture textures[texture_count] = {
        Texture("textures/161.png"),
        Texture("textures/211.png"),
        Texture("textures/212.png"),
    };

    static const int wall_count = 10;
    Wall walls[wall_count] = {
        Wall(&points[0], &points[1], Color(0, 100, 100), &textures[0], 1),
        Wall(&points[1], &points[2], Color(100, 100, 0), &textures[1]),
        Wall(&points[2], &points[3], Color(100, 0, 100), &textures[1]),
        Wall(&points[3], &points[4], Color(100, 100, 0), &textures[1]),
        Wall(&points[4], &points[0], Color(100, 0, 100), &textures[1]),

        Wall(&points[1], &points[0], Color(0, 100, 100), &textures[0], 0),
        Wall(&points[0], &points[7], Color(100, 100, 0), &textures[1]),
        Wall(&points[7], &points[6], Color(100, 0, 100), &textures[1]),
        Wall(&points[6], &points[5], Color(100, 100, 0), &textures[1]),
        Wall(&points[5], &points[1], Color(100, 0, 100), &textures[1]),
    };

    static const int sector_count = 2;
    Sector sectors[sector_count] = {
        Sector(5, new Wall*[5] {&walls[0], &walls[1], &walls[2], &walls[3], &walls[4]}, 0, 127, Color(64, 64, 255), Color(32, 32, 32)),
        Sector(5, new Wall*[5] {&walls[5], &walls[6], &walls[7], &walls[8], &walls[9]}, 0, 127, Color(64, 64, 255), Color(32, 32, 32)),
    };
};
