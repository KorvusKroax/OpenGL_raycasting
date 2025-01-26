#pragma once

#include <canvas/color.h>
#include "wall.h"

struct Sector
{
    int wall_count;
    Wall** walls;
    float bottom, top;
    Color floor_color;
    Color ceiling_color;
    Texture* floor_texture;
    Texture* ceiling_texture;

    Sector() { }

    Sector(int wall_count, Wall** walls, float bottom, float top, Color floor_color, Color ceiling_color, Texture* floor_texture, Texture* ceiling_texture)
    {
        this->wall_count = wall_count;
        this->walls = walls;
        this->bottom = bottom;
        this->top = top;
        this->floor_color = floor_color;
        this->ceiling_color = ceiling_color;
        this->floor_texture = floor_texture;
        this->ceiling_texture = ceiling_texture;
    }

    bool pointInSector(float x, float y)
    {
        double minX = (*(*walls[0]).start).x;
        double maxX = (*(*walls[0]).start).x;
        double minY = (*(*walls[0]).start).y;
        double maxY = (*(*walls[0]).start).y;
        for (int i = 1 ; i < wall_count; i++) {
            if (minX > (*(*walls[i]).start).x) minX = (*(*walls[i]).start).x;
            if (maxX < (*(*walls[i]).start).x) maxX = (*(*walls[i]).start).x;
            if (minY > (*(*walls[i]).start).y) minY = (*(*walls[i]).start).y;
            if (maxY < (*(*walls[i]).start).y) maxY = (*(*walls[i]).start).y;
        }
        if (x < minX || x > maxX || y < minY || y > maxY) return false;

        // https://wrf.ecse.rpi.edu/Research/Short_Notes/pnpoly.html
        bool result = false;
        for (int i = 0, j = wall_count - 1; i < wall_count; j = i++) {
            if ( (((*(*walls[i]).start).y > y) != ((*(*walls[j]).start).y > y)) &&
                 (x < ((*(*walls[j]).start).x - (*(*walls[i]).start).x) * (y - (*(*walls[i]).start).y) / ((*(*walls[j]).start).y - (*(*walls[i]).start).y) + (*(*walls[i]).start).x)
            ) {
                result = !result;
            }
        }

        return result;
    }

};
