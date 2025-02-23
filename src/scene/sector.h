#pragma once

#include <canvas/color.h>
#include "wall.h"
#include "texture.h"

struct Sector
{
    int wallCount;
    Wall** walls;
    float bottom, top;
    Texture* floorTexture;
    Texture* ceilingTexture;

    Sector() { }

    Sector(int wallCount, Wall** walls, float bottom, float top, Texture* floorTexture, Texture* ceilingTexture)
    {
        this->wallCount = wallCount;
        this->walls = walls;
        this->bottom = bottom;
        this->top = top;
        this->floorTexture = floorTexture;
        this->ceilingTexture = ceilingTexture;
    }

    bool pointInSector(Point p)
    {
        double minX = this->walls[0]->start->x;
        double minY = this->walls[0]->start->y;
        double maxX = this->walls[0]->start->x;
        double maxY = this->walls[0]->start->y;
        for (int i = 1 ; i < wallCount; i++) {
            if (minX > this->walls[i]->start->x) minX = this->walls[i]->start->x;
            if (minY > this->walls[i]->start->y) minY = this->walls[i]->start->y;
            if (maxX < this->walls[i]->start->x) maxX = this->walls[i]->start->x;
            if (maxY < this->walls[i]->start->y) maxY = this->walls[i]->start->y;
        }
        if (p.x < minX || p.x > maxX || p.y < minY || p.y > maxY) return false;

        // https://wrf.ecse.rpi.edu/Research/Short_Notes/pnpoly.html
        bool result = false;
        for (int i = 0, j = wallCount - 1; i < wallCount; j = i++) {
            if ( ((this->walls[i]->start->y > p.y) != (this->walls[j]->start->y > p.y)) &&
                 (p.x < (this->walls[j]->start->x - this->walls[i]->start->x) * (p.y - this->walls[i]->start->y) / (this->walls[j]->start->y - this->walls[i]->start->y) + this->walls[i]->start->x)
            ) {
                result = !result;
            }
        }

        return result;
    }
};
