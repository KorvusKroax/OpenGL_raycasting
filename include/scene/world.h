#pragma once

#include "point.h"
#include "wall.h"
#include "sector.h"
#include "texture.h"

struct World
{
    static const int point_count = 11;
    Point points[point_count] = {
        Point(-64, 100),
        Point(64, 100),

        Point(200, 50),
        Point(0, -200),
        Point(-200, -50),

        Point(200, 250),
        Point(0, 600),
        Point(-200, 300),
        Point(-200, 427),

        Point(-64, 224),
        Point(64, 224),
    };

    static const int texture_count = 8;
    Texture textures[texture_count] = {
        Texture("textures/161.png"),
        Texture("textures/211.png"),
        Texture("textures/212.png"),
        Texture("textures/117.png"),
        Texture("textures/373.png"),
        Texture("textures/091.png"),
        Texture("textures/122.png"),
        Texture("textures/547.png"),
    };

    static const int wall_count = 15;
    Wall walls[wall_count] = {
        Wall(&points[0], &points[1], Color(0, 150, 150), &textures[7], 2),
        Wall(&points[1], &points[2], Color(150, 150, 0), &textures[1]),
        Wall(&points[2], &points[3], Color(150, 0, 150), &textures[1]),
        Wall(&points[3], &points[4], Color(150, 150, 0), &textures[1]),
        Wall(&points[4], &points[0], Color(150, 0, 150), &textures[1]),

        Wall(&points[10], &points[9], Color(0, 100, 100), &textures[7], 2),
        Wall(&points[9], &points[7], Color(100, 100, 0), &textures[2]),
        Wall(&points[7], &points[8], Color(0,   100, 0), &textures[0]),
        Wall(&points[8], &points[6], Color(100, 0, 100), &textures[2]),
        Wall(&points[6], &points[5], Color(100, 100, 0), &textures[2]),
        Wall(&points[5], &points[10], Color(100, 0, 100), &textures[2]),

        Wall(&points[1], &points[0], Color(0, 150, 150), &textures[7], 0),
        Wall(&points[0], &points[9], Color(0, 150, 150), &textures[7]),
        Wall(&points[9], &points[10], Color(0, 100, 100), &textures[7], 1),
        Wall(&points[10], &points[1], Color(0, 100, 100), &textures[7]),
    };

    static const int sector_count = 3;
    Sector sectors[sector_count] = {
        Sector(5, new Wall*[5] {&walls[0], &walls[1], &walls[2], &walls[3], &walls[4]},             -50, 228, Color(63, 63, 63), Color(127, 127, 255), &textures[3], &textures[5]),
        Sector(6, new Wall*[6] {&walls[5], &walls[6], &walls[7], &walls[8], &walls[9], &walls[10]}, 0, 128, Color(31, 31, 31), Color(63, 63, 255)  , &textures[3], &textures[6]),
        Sector(4, new Wall*[4] {&walls[11], &walls[12], &walls[13], &walls[14]},                    20, 98, Color(10, 10, 10), Color(32, 32, 255)  , &textures[4], &textures[4]),
    };
};
