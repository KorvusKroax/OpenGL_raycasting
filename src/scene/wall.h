#pragma once

struct Wall
{
    int start;
    int end;
    int texture;
    int portalTo;

    Wall(int startPoint, int endPoint, int texture, int portalTo = -1)
    {
        this->start = startPoint;
        this->end = endPoint;
        this->texture = texture;
        this->portalTo = portalTo;
    }
};
