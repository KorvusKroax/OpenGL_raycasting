#pragma once

#include <canvas/canvas.h>
#include "world.h"
#include "viewpoint.h"
#include "misc.h"

namespace Sight
{
    void render_wireframe(Canvas* canvas, World* world, Viewpoint* viewpoint, float fov)
    {
        float fov_rad = Misc::deg2rad(fov);

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
                start.x = end.x - end.y * ((end.x - start.x) / (end.y - start.y));
                start.y = 0;
            } else if (end.y <= 0) {
                end.x = start.x - start.y * ((end.x - start.x) / (end.y - start.y));
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

    void render_raycast(Canvas* canvas, World* world, Viewpoint* viewpoint, float fov)
    {
        float fov_rad = Misc::deg2rad(fov);

        float nearClip = (canvas->width >> 1) / tan(fov_rad * .5f);

        float ray_dir_angle = -fov_rad * .5f;
        float ray_dir_angle_step = fov_rad / canvas->width;
        for (int col = 0; col < canvas->width; col++) {

            int wall_index = -1;
            float ix, iy, iDist = std::numeric_limits<float>::max();
            for (int i = 0; i < World::wall_count; i++) {
                Point start = (*world->walls[i].start).sub(viewpoint->pos);
                Point end = (*world->walls[i].end).sub(viewpoint->pos);

                float x, y, d;
                if (Misc::rayIntersection(start.x, start.y, end.x, end.y, sin(viewpoint->heading + ray_dir_angle), cos(viewpoint->heading + ray_dir_angle),  &x, &y)) {
                    d = x * x + y * y;
                    if (d < iDist) {
                        wall_index = i;
                        ix = x;
                        iy = y;
                        iDist = d;
                    }
                }
            }

            if (wall_index != -1) {
                float slice_dist = sqrt(iDist) * cos(ray_dir_angle) + 1;

                float wall_slice_height_top = (world->walls[wall_index].top - viewpoint->height) / slice_dist * nearClip;
                float wall_slice_height_bottom = (viewpoint->height - world->walls[wall_index].bottom) / slice_dist * nearClip;

                Point t_start = (*world->walls[wall_index].start).sub(viewpoint->pos);
                float t_dx = ix - t_start.x;
                float t_dy = iy - t_start.y;
                float texture_slice_x = sqrt(t_dx * t_dx + t_dy * t_dy);
                while (texture_slice_x >= world->textures[world->walls[wall_index].texture_id].width) {
                    texture_slice_x -= world->textures[world->walls[wall_index].texture_id].width;
                }

                float texture_step_y = (float)(world->walls[wall_index].top - world->walls[wall_index].bottom) / (wall_slice_height_top + wall_slice_height_bottom);
                float texture_slice_y = 0;
                // float f = (int)wall_slice_height_bottom * texture_step_y;
                // float texture_slice_y = 1.0f - abs(f - (int)f);

                for (int row = -wall_slice_height_bottom; row < wall_slice_height_top; row++) {

                    int color_index = (int)texture_slice_x + (int)texture_slice_y * world->textures[world->walls[wall_index].texture_id].width;
                    Color color = world->textures[world->walls[wall_index].texture_id].pixels[color_index];
                    canvas->setPixel(col, (canvas->height >> 1) + row, color);

                    texture_slice_y += texture_step_y;
                    if (texture_slice_y >= world->textures[world->walls[wall_index].texture_id].height) {
                        texture_slice_y -= world->textures[world->walls[wall_index].texture_id].height;
                    }
                }
            }

            ray_dir_angle += ray_dir_angle_step;
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
