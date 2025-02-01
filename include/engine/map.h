#pragma once

#include <canvas/canvas.h>
#include <scene/world.h>
#include <misc.h>
#include "camera.h"

namespace Map
{
    const float zoomStepping = 0.1f;
    float zoom = 1;
    bool map_is_active = false;

    void changeZoom(double yScroll)
    {
        if (!map_is_active) return;

        zoom += yScroll * zoomStepping;
        if (zoom < zoomStepping) zoom = zoomStepping;
    }

    void rotate(float x, float y, float angle, float* rotated_x, float* rotated_y)
    {
        *rotated_x = (x * cos(angle)) + (y * -sin(angle));
        *rotated_y = (x * sin(angle)) + (y * cos(angle));
    }

    void render(Canvas* canvas, World* world, Camera* camera, bool transformed = false)
    {
        for (Wall wall : world->walls) {
            float start_x, start_y;
            rotate(
                (*wall.start).x - camera->x,
                (*wall.start).y - camera->y,
                camera->heading,
                &start_x, &start_y
            );
            start_x *= zoom;
            start_y *= zoom;
            start_x += canvas->width >> 1;
            start_y += canvas->height >> 1;

            float end_x, end_y;
            rotate(
                (*wall.end).x - camera->x,
                (*wall.end).y - camera->y,
                camera->heading,
                &end_x, &end_y
            );
            end_x *= zoom;
            end_y *= zoom;
            end_x += canvas->width >> 1;
            end_y += canvas->height >> 1;

            Line::draw(canvas, start_x, start_y, end_x, end_y, Color(0, 255, 255));

            // wall's normal
            float dx = end_x - start_x;
            float dy = end_y - start_y;
            float wall_length = sqrt(dx * dx + dy * dy);
            float center_x = (start_x + end_x) * .5f;
            float center_y = (start_y + end_y) * .5f;
            float normal_dir_x = (dy / wall_length) * 5 * zoom;
            float normal_dir_y = (dx / wall_length) * -5 * zoom;

            Line::draw(canvas, center_x, center_y, center_x + normal_dir_x, center_y + normal_dir_y, Color(0, 255, 255));
        }
    }

    void render_camera(Canvas* canvas, World* world, Camera* camera, float fov, bool transformed = true)
    {
        float dir_length = 10.0f;
        float dir_rad = transformed ? camera->heading : 0;
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

        // camera (screen center)
        Line::draw(canvas, (canvas->width >> 1), (canvas->height >> 1), (canvas->width >> 1) + dir_x, (canvas->height >> 1) + dir_y, Color(255, 127, 0));
        Circle::draw_filled(canvas, (canvas->width >> 1), (canvas->height >> 1), 2 * zoom, Color(255, 127, 0));
    }
}
