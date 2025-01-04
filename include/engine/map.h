#pragma once

#include <canvas/canvas.h>
#include "world.h"
#include "viewpoint.h"

#define PI 3.141592653589793238463

namespace Map
{
    void render(Canvas* canvas, World* world, Viewpoint* viewpoint, float zoom)
    {
        for (Wall wall : world->walls) {
            Point start = (*wall.start)
                .sub(viewpoint->pos)
                .mult(zoom)
                .add(canvas->width >> 1, canvas->height >> 1);
            Point end = (*wall.end)
                .sub(viewpoint->pos)
                .mult(zoom)
                .add(canvas->width >> 1, canvas->height >> 1);

            Line::draw(canvas, start.x, start.y, end.x, end.y, Color(63, 63, 63));

            // wall's normal
            float dx = end.x - start.x;
            float dy = end.y - start.y;
            float wall_length = sqrt(dx * dx + dy * dy);
            Point center = start.add(end).mult(.5f);
            Point normal = Point(
                center.x + (dy / wall_length) * 5 * zoom,
                center.y + (dx / wall_length) * -5 * zoom
            );

            Line::draw(canvas, center.x, center.y, normal.x, normal.y, Color(63, 63, 63));
        }
    }

    void render_viewpoint(Canvas* canvas, World* world, Viewpoint* viewpoint, float zoom, float fov)
    {
        float viewDist = 100.0f;

        float fov_rad = fov / 180.0 * PI;
        float fov_left_x = sin(-fov_rad * .5f + viewpoint->heading) * viewDist * zoom;
        float fov_left_y = cos(-fov_rad * .5f + viewpoint->heading) * viewDist * zoom;
        float fov_right_x = sin(fov_rad * .5f + viewpoint->heading) * viewDist * zoom;
        float fov_right_y = cos(fov_rad * .5f + viewpoint->heading) * viewDist * zoom;

        float dir_x = sin(viewpoint->heading) * viewDist * zoom;
        float dir_y = cos(viewpoint->heading) * viewDist * zoom;

        // fov lines
        Line::draw(canvas, (canvas->width >> 1), (canvas->height >> 1), (canvas->width >> 1) + fov_left_x, (canvas->height >> 1) + fov_left_y, Color(63, 0, 0));
        Line::draw(canvas, (canvas->width >> 1), (canvas->height >> 1), (canvas->width >> 1) + fov_right_x, (canvas->height >> 1) + fov_right_y, Color(63, 0, 0));

        // viewpoint (screen center)
        Line::draw(canvas, (canvas->width >> 1), (canvas->height >> 1), (canvas->width >> 1) + dir_x, (canvas->height >> 1) + dir_y, Color(255, 0, 0));
        Circle::draw_filled(canvas, (canvas->width >> 1), (canvas->height >> 1), 2, Color(255, 0, 0));
    }




    void render_transformed(Canvas* canvas, World* world, Viewpoint* viewpoint, float zoom)
    {
        for (Wall wall : world->walls) {
            Point start = (*wall.start)
                .sub(viewpoint->pos)
                .rotate(viewpoint->heading)
                .mult(zoom)
                .add(canvas->width >> 1, canvas->height >> 1);
            Point end = (*wall.end)
                .sub(viewpoint->pos)
                .rotate(viewpoint->heading)
                .mult(zoom)
                .add(canvas->width >> 1, canvas->height >> 1);

            Line::draw(canvas, start.x, start.y, end.x, end.y, Color(63, 63, 63));

            // wall's normal
            float dx = end.x - start.x;
            float dy = end.y - start.y;
            float wall_length = sqrt(dx * dx + dy * dy);
            Point center = start.add(end).mult(.5f);
            Point normal = Point(
                center.x + (dy / wall_length) * 5 * zoom,
                center.y + (dx / wall_length) * -5 * zoom
            );

            Line::draw(canvas, center.x, center.y, normal.x, normal.y, Color(63, 63, 63));
        }
    }

    void render_transformed_sight(Canvas* canvas, World* world, Viewpoint* viewpoint, float zoom)
    {
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

            start = start
                .mult(zoom)
                .add(canvas->width >> 1, canvas->height >> 1);
            end = end
                .mult(zoom)
                .add(canvas->width >> 1, canvas->height >> 1);

            Line::draw(canvas, start.x, start.y, end.x, end.y, Color(192, 192, 192));
        }
    }

    void render_transformed_viewpoint(Canvas* canvas, World* world, Viewpoint* viewpoint, float zoom, float fov)
    {
        float viewDist = 100.0f;

        float fov_rad = fov / 180.0 * PI;
        float fov_left_x = sin(-fov_rad * .5f) * viewDist * zoom;
        float fov_left_y = cos(-fov_rad * .5f) * viewDist * zoom;
        float fov_right_x = sin(fov_rad * .5f) * viewDist * zoom;
        float fov_right_y = cos(fov_rad * .5f) * viewDist * zoom;

        // fov lines
        Line::draw(canvas, (canvas->width >> 1), (canvas->height >> 1), (canvas->width >> 1) + fov_left_x, (canvas->height >> 1) + fov_left_y, Color(63, 0, 0));
        Line::draw(canvas, (canvas->width >> 1), (canvas->height >> 1), (canvas->width >> 1) + fov_right_x, (canvas->height >> 1) + fov_right_y, Color(63, 0, 0));

        // viewpoint (screen center)
        Line::draw(canvas, (canvas->width >> 1), (canvas->height >> 1), (canvas->width >> 1), (canvas->height >> 1) + viewDist * zoom, Color(128, 0, 0));
        Circle::draw_filled(canvas, (canvas->width >> 1), (canvas->height >> 1), 2, Color(255, 0, 0));
    }
}
