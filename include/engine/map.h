#pragma once

#include <canvas/canvas.h>
#include "world.h"
#include "viewpoint.h"
#include "misc.h"


namespace Map
{
    const float zoomStepping = 0.1f;
    float zoom = 1;

    void zoom_callback(GLFWwindow* window, double xOffset, double yOffset)
    {
        zoom += yOffset * zoomStepping;
        if (zoom < zoomStepping) zoom = zoomStepping;
    }



    void render(Canvas* canvas, World* world, Viewpoint* viewpoint, bool transformed = false)
    {
        for (Wall wall : world->walls) {
            Point start = (*wall.start)
                .sub(viewpoint->pos)
                .rotate(transformed ? viewpoint->heading : 0)
                .mult(zoom)
                .add(canvas->width >> 1, canvas->height >> 1);
            Point end = (*wall.end)
                .sub(viewpoint->pos)
                .rotate(transformed ? viewpoint->heading : 0)
                .mult(zoom)
                .add(canvas->width >> 1, canvas->height >> 1);

            Line::draw(canvas, start.x, start.y, end.x, end.y, Color(0, 255, 255));

            // wall's normal
            float dx = end.x - start.x;
            float dy = end.y - start.y;
            float wall_length = sqrt(dx * dx + dy * dy);
            Point center = start.add(end).mult(.5f);
            Point normal = Point(
                center.x + (dy / wall_length) * 5 * zoom,
                center.y + (dx / wall_length) * -5 * zoom
            );

            Line::draw(canvas, center.x, center.y, normal.x, normal.y, Color(0, 255, 255));
        }
    }

    void render_viewpoint(Canvas* canvas, World* world, Viewpoint* viewpoint, float fov, bool transformed = true)
    {
        float dir_length = 10.0f;
        float dir_rad = transformed ? viewpoint->heading : 0;
        float dir_x = sin(dir_rad) * dir_length * zoom;
        float dir_y = cos(dir_rad) * dir_length * zoom;

        float fov_length = 100.0f;
        float fov_left_x = sin(-fov * .5f + dir_rad) * fov_length * zoom;
        float fov_left_y = cos(-fov * .5f + dir_rad) * fov_length * zoom;
        float fov_right_x = sin(fov * .5f + dir_rad) * fov_length * zoom;
        float fov_right_y = cos(fov * .5f + dir_rad) * fov_length * zoom;

        // fov lines
        Line::draw(canvas, (canvas->width >> 1), (canvas->height >> 1), (canvas->width >> 1) + fov_left_x, (canvas->height >> 1) + fov_left_y, Color(255, 127, 0, 127));
        Line::draw(canvas, (canvas->width >> 1), (canvas->height >> 1), (canvas->width >> 1) + fov_right_x, (canvas->height >> 1) + fov_right_y, Color(255, 127, 0, 127));

        // viewpoint (screen center)
        Line::draw(canvas, (canvas->width >> 1), (canvas->height >> 1), (canvas->width >> 1) + dir_x, (canvas->height >> 1) + dir_y, Color(255, 127, 0));
        Circle::draw_filled(canvas, (canvas->width >> 1), (canvas->height >> 1), 2 * zoom, Color(255, 127, 0));
    }



    void render_transformed_sight(Canvas* canvas, World* world, Viewpoint* viewpoint)
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
                start.x = end.x - end.y * ((end.x - start.x) / (end.y - start.y));
                start.y = 0;
            } else if (end.y <= 0) {
                end.x = start.x - start.y * ((end.x - start.x) / (end.y - start.y));
                end.y = 0;
            }

            start = start
                .mult(zoom)
                .add(canvas->width >> 1, canvas->height >> 1);
            end = end
                .mult(zoom)
                .add(canvas->width >> 1, canvas->height >> 1);

            Line::draw(canvas, start.x, start.y, end.x, end.y, Color(255, 255, 255));
        }
    }
}
