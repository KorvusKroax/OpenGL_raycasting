#pragma once

struct Wall
{
    int start;
    int end;
    int texture;
    int portalTo;

    Wall() { }

    Wall(int startPoint, int endPoint, int texture, int portalTo)
    {
        this->start = startPoint;
        this->end = endPoint;
        this->texture = texture;
        this->portalTo = portalTo;
    }

    Wall(int *serialized_sector[])
    {
        this->start = (*serialized_sector)[0];
        this->end = (*serialized_sector)[1];
        this->texture = (*serialized_sector)[2];
        this->portalTo = (*serialized_sector)[3];
    }

    int serialize(int *serialized_wall[])
    {
        int size = 4;
        *serialized_wall = new int[size] {start, end, texture, portalTo};

        return size;
    }
};
