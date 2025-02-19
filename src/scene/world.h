#pragma once

#include "point.h"
#include "wall.h"
#include "sector.h"
#include "texture.h"

struct World
{
    Point points[11] = {
        Point(-64, 100),
        Point(64, 100),

        Point(200, 50),
        Point(0, -600),
        Point(-200, -50),

        Point(200, 250),
        Point(0, 600),
        Point(-200, 300),
        Point(-200, 427),

        Point(-64, 224),
        Point(64, 224),
    };

    Texture textures[8] = {
        Texture("textures/161.png"),
        Texture("textures/211.png"),
        Texture("textures/212.png"),
        Texture("textures/117.png"),
        Texture("textures/373.png"),
        Texture("textures/091.png"),
        Texture("textures/122.png"),
        Texture("textures/547.png"),
    };

    Wall walls[15] = {
        Wall(&points[0], &points[1], &textures[7], 2),
        Wall(&points[1], &points[2], &textures[1]),
        Wall(&points[2], &points[3], &textures[1]),
        Wall(&points[3], &points[4], &textures[1]),
        Wall(&points[4], &points[0], &textures[1]),

        Wall(&points[10], &points[9], &textures[7], 2),
        Wall(&points[9], &points[7], &textures[2]),
        Wall(&points[7], &points[8], &textures[0]),
        Wall(&points[8], &points[6], &textures[2]),
        Wall(&points[6], &points[5], &textures[2]),
        Wall(&points[5], &points[10], &textures[2]),

        Wall(&points[1], &points[0], &textures[7], 0),
        Wall(&points[0], &points[9], &textures[7]),
        Wall(&points[9], &points[10], &textures[7], 1),
        Wall(&points[10], &points[1], &textures[7]),
    };

    Sector sectors[3] = {
        Sector(5, new Wall*[5] {&walls[0], &walls[1], &walls[2], &walls[3], &walls[4]},             -50, 228, &textures[3], &textures[5]),
        Sector(6, new Wall*[6] {&walls[5], &walls[6], &walls[7], &walls[8], &walls[9], &walls[10]},   0, 128, &textures[3], &textures[5]),
        Sector(4, new Wall*[4] {&walls[11], &walls[12], &walls[13], &walls[14]},                     20,  98, &textures[4], &textures[4]),
    };
};
