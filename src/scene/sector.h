#pragma once

struct Sector
{
    int wallCount;
    int* walls;
    float bottom, top;
    int floorTexture;
    int ceilingTexture;

    Sector() { }

    Sector(int wallCount, int* walls, float bottom, float top, int floorTexture, int ceilingTexture)
    {
        this->wallCount = wallCount;
        this->walls = walls;
        this->bottom = bottom;
        this->top = top;
        this->floorTexture = floorTexture;
        this->ceilingTexture = ceilingTexture;
    }
};
