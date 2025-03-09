#pragma once

struct Sector
{
    int wallCount;
    int *walls;
    float bottom, top;
    int floorTexture;
    int ceilingTexture;

    Sector() { }

    Sector(int wallCount, int *walls, float bottom, float top, int floorTexture, int ceilingTexture)
    {
        this->wallCount = wallCount;
        this->walls = walls;
        this->bottom = bottom;
        this->top = top;
        this->floorTexture = floorTexture;
        this->ceilingTexture = ceilingTexture;
    }

    Sector(int *serialized_sector[])
    {
        this->wallCount = (*serialized_sector)[0];
        this->walls = new int[this->wallCount];
        for (int i = 0; i < this->wallCount; i++) this->walls[i] = (*serialized_sector)[1 + i];
        this->bottom = (*serialized_sector)[this->wallCount + 1] / 1000.0f;
        this->top = (*serialized_sector)[this->wallCount + 2] / 1000.0f;
        this->floorTexture = (*serialized_sector)[this->wallCount + 3];
        this->ceilingTexture = (*serialized_sector)[this->wallCount + 4];
    }

    int serialize(int *serialized_sector[])
    {
        int size = 5 + this->wallCount;
        *serialized_sector = new int[size];

        (*serialized_sector)[0] = this->wallCount;
        for (int i = 0; i < this->wallCount; i++) (*serialized_sector)[1 + i] = this->walls[i];
        (*serialized_sector)[this->wallCount + 1] = int(this->bottom * 1000);
        (*serialized_sector)[this->wallCount + 2] = int(this->top * 1000);
        (*serialized_sector)[this->wallCount + 3] = this->floorTexture;
        (*serialized_sector)[this->wallCount + 4] = this->ceilingTexture;

        return size;
    }
};
