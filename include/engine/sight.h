#pragma once

#include <canvas/canvas.h>
#include "world.h"
#include "viewpoint.h"
#include "misc.h"

namespace Sight
{
/*
    void render_wireframe(Canvas* canvas, World* world, Viewpoint* viewpoint, float fov)
    {
        float near_clip = (canvas->width >> 1) / tan(fov * .5f);

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
            float px1 = (x1 / z1) * near_clip + (canvas->width >> 1);
            float py1 = (y1 / z1) * near_clip + (canvas->height >> 1);

            float px2 = (x2 / z2) * near_clip + (canvas->width >> 1);
            float py2 = (y2 / z2) * near_clip + (canvas->height >> 1);

            float px3 = (x3 / z3) * near_clip + (canvas->width >> 1);
            float py3 = (y3 / z3) * near_clip + (canvas->height >> 1);

            float px4 = (x4 / z4) * near_clip + (canvas->width >> 1);
            float py4 = (y4 / z4) * near_clip + (canvas->height >> 1);

            // draw wall
            Line::draw(canvas, px1, py1, px2, py2, Color(0, 192, 192));
            Line::draw(canvas, px2, py2, px3, py3, Color(0, 192, 192));
            Line::draw(canvas, px3, py3, px4, py4, Color(0, 192, 192));
            Line::draw(canvas, px4, py4, px1, py1, Color(0, 192, 192));
        }
    }
*/

    void render_raycast_wall_slice(Canvas* canvas, int col, Wall* wall, float wall_slice_bottom, float wall_slice_top)
    {
        for (int row = wall_slice_bottom; row < wall_slice_top; row++) {
            canvas->setPixel(col, row, wall->color);
        }
    }

    void render_raycast_textured_wall_slice(Canvas* canvas, int col, Wall* wall, Viewpoint* viewpoint, float sector_height, float wall_slice_bottom, float wall_slice_top, Point rayIntersection)
    {
        Point t_start = (*(*wall).start).sub(viewpoint->pos).rotate(viewpoint->heading);
        float t_dx = rayIntersection.x - t_start.x;
        float t_dy = rayIntersection.y - t_start.y;
        float texture_slice_x = fmod(sqrt(t_dx * t_dx + t_dy * t_dy), (*(*wall).texture).width);

        float texture_step_y =  sector_height / (wall_slice_top - wall_slice_bottom);
        float texture_slice_y = 0;
        // float f = (int)wall_slice_bottom * texture_step_y;
        // float texture_slice_y = 1.0f - abs(f - (int)f);

        for (int row = wall_slice_bottom; row < wall_slice_top; row++) {
            int color_index = (int)texture_slice_x + (int)texture_slice_y * (*(*wall).texture).width;
            canvas->setPixel(col, row, (*(*wall).texture).pixels[color_index]);

            texture_slice_y += texture_step_y;
            if (texture_slice_y >= (*(*wall).texture).height) {
                texture_slice_y -= (*(*wall).texture).height;
            }
        }
    }



    void render_raycast_floor(Canvas* canvas, int col, Sector* sector, float slice_bottom, float wall_slice_bottom)
    {
        for (int row = slice_bottom; row < wall_slice_bottom - 1; row++) {
            canvas->setPixel(col, row, sector->floor_color);
        }
    }

    void render_raycast_textured_floor(Canvas* canvas, int col, Sector* sector, Viewpoint* viewpoint, float ray_dir_angle, float near_clip, float horizont_line, float slice_bottom, float wall_slice_bottom)
    {
        for (int row = slice_bottom; row < wall_slice_bottom - 1; row++) {

            float t_dist = viewpoint->height / (row - horizont_line) * near_clip / cos(ray_dir_angle);

            float t_pos_x = Misc::nmod(viewpoint->pos.x - t_dist * sin(viewpoint->heading + ray_dir_angle), (*(*sector).floor_texture).width);
            float t_pos_y = Misc::nmod(viewpoint->pos.y - t_dist * cos(viewpoint->heading + ray_dir_angle), (*(*sector).floor_texture).height);

            int color_index = (int)t_pos_x + (int)t_pos_y * (*(*sector).floor_texture).width;
            canvas->setPixel(col, row, (*(*sector).floor_texture).pixels[color_index]);
        }
    }



    void render_raycast_ceiling(Canvas* canvas, int col, Sector* sector, float slice_top, float wall_slice_top)
    {
        for (int row = wall_slice_top + 1; row < slice_top; row++) {
            canvas->setPixel(col, row, sector->ceiling_color);
        }
    }

    void render_raycast_textured_ceiling(Canvas* canvas, int col, Sector* sector, Viewpoint* viewpoint, float ray_dir_angle, float near_clip, float horizont_line, float slice_top, float wall_slice_top)
    {
        for (int row = wall_slice_top + 1; row < slice_top; row++) {

            float t_dist = (((*sector).top - (*sector).bottom) - viewpoint->height) / (horizont_line - row) * near_clip / cos(ray_dir_angle);

            float t_pos_x = Misc::nmod(viewpoint->pos.x - t_dist * sin(viewpoint->heading + ray_dir_angle), (*(*sector).ceiling_texture).width);
            float t_pos_y = Misc::nmod(viewpoint->pos.y - t_dist * cos(viewpoint->heading + ray_dir_angle), (*(*sector).ceiling_texture).height);

            int color_index = (int)t_pos_x + (int)t_pos_y * (*(*sector).ceiling_texture).width;
            canvas->setPixel(col, row, (*(*sector).ceiling_texture).pixels[color_index]);
        }
    }



    void render_raycast_column(Canvas* canvas, int col, World* world, Sector* sector, Viewpoint* viewpoint, float ray_dir_angle, float near_clip, float slice_bottom, float slice_top)
    {
        int wall_index = -1;
        float ix, iy, iDist = std::numeric_limits<float>::max();

        for (int i = 0; i < (*sector).wall_count; i++) {
            Point start = (*(*(*sector).walls[i]).start).sub(viewpoint->pos).rotate(viewpoint->heading);
            Point end = (*(*(*sector).walls[i]).end).sub(viewpoint->pos).rotate(viewpoint->heading);

            if (atan2(start.x, start.y) > atan2(end.x, end.y)) continue;

            float x, y, d;
            if (Misc::rayIntersection(start.x, start.y, end.x, end.y, sin(ray_dir_angle), cos(ray_dir_angle),  &x, &y)) {
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
            float horizont_line = (canvas->height >> 1) + viewpoint->pitch;

            float wall_slice_top = horizont_line + ((*sector).top - viewpoint->height) / slice_dist * near_clip;
            float wall_slice_bottom = horizont_line - (viewpoint->height - (*sector).bottom) / slice_dist * near_clip;

            if ((*(*sector).walls[wall_index]).portal_to != -1) {
                render_raycast_column(canvas, col, world, &world->sectors[(*(*sector).walls[wall_index]).portal_to], viewpoint, ray_dir_angle, near_clip, wall_slice_bottom, wall_slice_top);
            } else {
                // render_raycast_wall_slice(canvas, col, (*sector).walls[wall_index], wall_slice_bottom, wall_slice_top);
                render_raycast_textured_wall_slice(canvas, col, (*sector).walls[wall_index], viewpoint, (*sector).top - (*sector).bottom, wall_slice_bottom, wall_slice_top, Point(ix, iy));
            }

            // render_raycast_floor(canvas, col, sector, slice_bottom, wall_slice_bottom);
            render_raycast_textured_floor(canvas, col, sector, viewpoint, ray_dir_angle, near_clip, horizont_line, slice_bottom, wall_slice_bottom);

            // render_raycast_ceiling(canvas, col, sector, slice_top, wall_slice_top);
            render_raycast_textured_ceiling(canvas, col, sector, viewpoint, ray_dir_angle, near_clip, horizont_line, slice_top, wall_slice_top);
        }
    }

    void render_raycast(Canvas* canvas, World* world, Viewpoint* viewpoint, float fov)
    {
        float near_clip = (canvas->width >> 1) / tan(fov * .5f);

        int curr_sector = -1;
        for (int i = 0; i < world->sector_count; i++) {
            if (world->sectors[i].pointInSector(viewpoint->pos.x, viewpoint->pos.y)) {
                curr_sector = i;
                break;
            }
        }

        if (curr_sector == -1) return;

        float ray_dir_angle = -fov * .5f;
        float ray_dir_angle_step = fov / canvas->width;
        for (int col = 0; col < canvas->width; col++) {
            render_raycast_column(canvas, col, world, &world->sectors[curr_sector], viewpoint, ray_dir_angle, near_clip, 0, canvas->height);
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
