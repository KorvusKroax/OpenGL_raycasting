#pragma once

#include "texture.h"
#include "point.h"
#include "wall.h"
#include "sector.h"

struct World
{
    Texture textures[8] = {
        Texture("textures/161.png"),
        Texture("textures/211.png"),
        Texture("textures/212.png"),
        Texture("textures/117.png"),
        Texture("textures/373.png"),
        Texture("textures/091.png"),
        Texture("textures/547.png"),
    };

    Point points[11] = {
        Point(-64, 100),
        Point(64, 100),

        Point(200, 50),
        Point(0, -600),
        Point(-200, -50),

        Point(200, 300),
        Point(0, 600),
        Point(-200, 300),
        Point(-200, 427),

        Point(-64, 224),
        Point(64, 224),
    };

    Wall walls[15] = {
        Wall(0, 1, 6, 2),
        Wall(1, 2, 1),
        Wall(2, 3, 1),
        Wall(3, 4, 1),
        Wall(4, 0, 1),

        Wall(10, 9, 6, 2),
        Wall(9, 7, 2),
        Wall(7, 8, 0),
        Wall(8, 6, 2),
        Wall(6, 5, 2),
        Wall(5, 10, 2),

        Wall(1, 0, 6, 0),
        Wall(0, 9, 6),
        Wall(9, 10, 6, 1),
        Wall(10, 1, 6),
    };

    Sector sectors[3] = {
        Sector(5, new int[5] {0, 1, 2, 3, 4},     -50, 228, 3, 5),
        Sector(6, new int[6] {5, 6, 7, 8, 9, 10},   0, 128, 3, 5),
        Sector(4, new int[4] {11, 12, 13, 14},     20,  98, 4, 4),
    };
};
