#pragma once

#include <canvas/canvas.h>
#include "world.h"
#include "viewpoint.h"

#define PI 3.141592653589793238463

namespace Sight
{
    void render(Canvas* canvas, World* world, Viewpoint* viewpoint, float fov)
    {
        float fov_rad = fov / 180.0 * PI;
        float nearClip = (canvas->width >> 1) / tan(fov_rad * .5f);

        for (Wall wall : world->walls) {
            Point start = (*wall.start)
                .sub(viewpoint->pos)
                .rotate(viewpoint->heading);
            Point end = (*wall.end)
                .sub(viewpoint->pos)
                .rotate(viewpoint->heading);

            if (start.y <= 0 && end.y <= 0) continue;

            // clip behind
            if (start.y <= 0) {
                float dx = end.x - start.x;
                float dy = end.y - start.y;
                start.x = end.x - end.y * (dx / dy);
                start.y = 0;
            } else if (end.y <= 0) {
                float dx = end.x - start.x;
                float dy = end.y - start.y;
                end.x = start.x - start.y * (dx / dy);
                end.y = 0;
            }

            // calculate wall vertices
            float x1 = start.x;
            float y1 = wall.top - viewpoint->height;
            float z1 = start.y + 1;

            float x2 = end.x;
            float y2 = wall.top - viewpoint->height;
            float z2 = end.y + 1;

            float x3 = end.x;
            float y3 = wall.bottom - viewpoint->height;
            float z3 = end.y + 1;

            float x4 = start.x;
            float y4 = wall.bottom - viewpoint->height;
            float z4 = start.y + 1;

            // calculate vertices' screen coords
            float px1 = (x1 / z1) * nearClip + (canvas->width >> 1);
            float py1 = (y1 / z1) * nearClip + (canvas->height >> 1);

            float px2 = (x2 / z2) * nearClip + (canvas->width >> 1);
            float py2 = (y2 / z2) * nearClip + (canvas->height >> 1);

            float px3 = (x3 / z3) * nearClip + (canvas->width >> 1);
            float py3 = (y3 / z3) * nearClip + (canvas->height >> 1);

            float px4 = (x4 / z4) * nearClip + (canvas->width >> 1);
            float py4 = (y4 / z4) * nearClip + (canvas->height >> 1);

            // draw wall
            Line::draw(canvas, px1, py1, px2, py2, Color(0, 192, 192));
            Line::draw(canvas, px2, py2, px3, py3, Color(0, 192, 192));
            Line::draw(canvas, px3, py3, px4, py4, Color(0, 192, 192));
            Line::draw(canvas, px4, py4, px1, py1, Color(0, 192, 192));
        }
    }
}



/*
    void renderView_3d_wireframe()
    {
        float fov = 80 / 180.0f * pi;
        float zFar = 1000.0f;
        float zNear = 10.0f;

        float aspectRatio = (float)canvas->height / canvas->width;
        float f = 1.0f / tan(fov * .5f);
        float q = zFar / (zFar - zNear);

        float fov_x1 = sin(-fov * .5f) * zFar;
        float fov_y1 = cos(-fov * .5f) * zFar;
        float fov_x2 = sin(fov * .5f) * zFar;
        float fov_y2 = cos(fov * .5f) * zFar;

        for (Wall wall : world.walls) {

            // transform wall
            Point start = world.points[wall.start]
                .sub(viewpoint.pos)
                .rotate(viewpoint.heading);
            Point end = world.points[wall.end]
                .sub(viewpoint.pos)
                .rotate(viewpoint.heading);

            // cut wall to view
            // ...

            // calculate wall vertices
            float x1 = aspectRatio * f * start.x;
            float y1 = f * (wall.top - viewpoint.height);
            float z1 = start.y * q - zNear * q;

            float x2 = aspectRatio * f * end.x;
            float y2 = f * (wall.top - viewpoint.height);
            float z2 = end.y * q - zNear * q;

            float x3 = aspectRatio * f * end.x;
            float y3 = f * (wall.bottom - viewpoint.height);
            float z3 = end.y * q - zNear * q;

            float x4 = aspectRatio * f * start.x;
            float y4 = f * (wall.bottom - viewpoint.height);
            float z4 = start.y * q - zNear * q;

            // calculate vertices' screen coords
            float px1 = (x1 / z1 + 1) * (canvas->width >> 1);
            float py1 = (y1 / z1 + 1) * (canvas->height >> 1);

            float px2 = (x2 / z2 + 1) * (canvas->width >> 1);
            float py2 = (y2 / z2 + 1) * (canvas->height >> 1);

            float px3 = (x3 / z3 + 1) * (canvas->width >> 1);
            float py3 = (y3 / z3 + 1) * (canvas->height >> 1);

            float px4 = (x4 / z4 + 1) * (canvas->width >> 1);
            float py4 = (y4 / z4 + 1) * (canvas->height >> 1);

            // draw wall
            drawLine(px1, py1, px2, py2, ColorRGBA(128, 128, 0));
            drawLine(px2, py2, px3, py3, ColorRGBA(128, 128, 0));
            drawLine(px3, py3, px4, py4, ColorRGBA(128, 128, 0));
            drawLine(px4, py4, px1, py1, ColorRGBA(128, 128, 0));
        }
    }
*/
