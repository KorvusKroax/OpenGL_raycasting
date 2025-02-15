#pragma once

#include <canvas/canvas.h>
#include <scene/world.h>
#include <misc.h>
#include "camera.h"

namespace View
{
    void rotate(float x, float y, float angle, float* rotated_x, float* rotated_y)
    {
        *rotated_x = (x * cos(angle)) + (y * -sin(angle));
        *rotated_y = (x * sin(angle)) + (y * cos(angle));
    }

    void render_wall_slice(Canvas* canvas, int col, Wall* wall, Camera* camera, float wall_height, float wall_slice_bottom, float wall_slice_top, float portal_bottom, float portal_top, float ray_intersection_x, float ray_intersection_y)//, float ray_intersection_distance)
    {
        float wall_slice_clipped_bottom = wall_slice_bottom > portal_bottom ? wall_slice_bottom : portal_bottom;
        float wall_slice_clipped_top = wall_slice_top < portal_top ? wall_slice_top : portal_top;

        float texture_pos_x, texture_pos_y;

        rotate(wall->start->x - camera->x, wall->start->y - camera->y, camera->heading, &texture_pos_x, &texture_pos_y);

        float texture_dx = ray_intersection_x - texture_pos_x;
        float texture_dy = ray_intersection_y - texture_pos_y;
        float texture_slice_x = fmod(sqrt(texture_dx * texture_dx + texture_dy * texture_dy), wall->texture->width);

        float texture_step_y =  wall_height / (wall_slice_top - wall_slice_bottom);

        float texture_slice_y = wall_slice_bottom < wall_slice_clipped_bottom ? (portal_bottom - wall_slice_bottom) * texture_step_y : 0;

        // float multiplier = 255.0f;
        // float intensity = .25f/ray_intersection_distance * multiplier;

        for (int row = 0; row <= (int)wall_slice_clipped_top - (int)wall_slice_clipped_bottom; row++) {

            int ty = fmod(texture_slice_y + texture_step_y * row, wall->texture->height);
            int color_index = (int)texture_slice_x + ty * wall->texture->width;

            Color color = wall->texture->pixels[color_index];
            // float r = color.getRed() * intensity;
            // float g = color.getGreen() * intensity;
            // float b = color.getBlue() * intensity;
            // color = Color(r < 255 ? r : 255, g < 255 ? g : 255, b < 255 ? b : 255);

            canvas->setPixel(col, wall_slice_clipped_bottom + row, color);
        }
    }

    void render_floor(Canvas* canvas, int col, Sector* sector, Camera* camera, float ray_dir_angle, float near_clip, float portal_slice_bottom, float portal_slice_top, float wall_slice_bottom)
    {
        wall_slice_bottom--;

        if (wall_slice_bottom < 0) return;

        float portal_slice_clipped_bottom = portal_slice_bottom > 0 ? portal_slice_bottom : 0;
        float wall_slice_clipped_bottom = wall_slice_bottom < portal_slice_top ? wall_slice_bottom : portal_slice_top;

        float horizont_line = (canvas->height >> 1) + camera->pitch;

        for (int row = portal_slice_clipped_bottom; row < wall_slice_clipped_bottom; row++) {

            float texture_dist = (camera->height - sector->bottom) / (row - horizont_line) * near_clip / cos(ray_dir_angle);

            float texture_pos_x = Misc::nmod(camera->x - texture_dist * sin(camera->heading + ray_dir_angle), sector->floor_texture->width);
            float texture_pos_y = Misc::nmod(camera->y - texture_dist * cos(camera->heading + ray_dir_angle), sector->floor_texture->height);

            int color_index = (int)texture_pos_x + (int)texture_pos_y * sector->floor_texture->width;
            Color color = sector->floor_texture->pixels[color_index];
            canvas->setPixel(col, row, color);
        }
    }

    void render_ceiling(Canvas* canvas, int col, Sector* sector, Camera* camera, float ray_dir_angle, float near_clip, float portal_slice_bottom, float portal_slice_top, float wall_slice_top)
    {
        wall_slice_top++;

        if (wall_slice_top > canvas->height) return;

        float portal_slice_clipped_top = portal_slice_top < canvas->height ? portal_slice_top : canvas->height;
        float wall_slice_clipped_top = wall_slice_top > portal_slice_bottom ? wall_slice_top : portal_slice_bottom;

        float horizont_line = (canvas->height >> 1) + camera->pitch;

        for (int row = wall_slice_clipped_top; row < portal_slice_clipped_top; row++) {

            float texture_dist = ((sector->top - sector->bottom) - (camera->height - sector->bottom)) / (horizont_line - row) * near_clip / cos(ray_dir_angle);

            float texture_pos_x = Misc::nmod(camera->x - texture_dist * sin(camera->heading + ray_dir_angle), sector->ceiling_texture->width);
            float texture_pos_y = Misc::nmod(camera->y - texture_dist * cos(camera->heading + ray_dir_angle), sector->ceiling_texture->height);

            int color_index = (int)texture_pos_x + (int)texture_pos_y * sector->ceiling_texture->width;
            Color color = sector->ceiling_texture->pixels[color_index];
            canvas->setPixel(col, row, color);
        }
    }

    inline bool backface_culling(float x1, float y1, float x2, float y2)
    {
        return (y2 - y1) * x1 - (x2 - x1) * y1 >= 0;
        // result > 0: origo is left from the line
        // result = 0: origo is on the line
        // result < 0: origo is right from the line
    }

    void render_column(Canvas* canvas, int col, World* world, Sector* sector, Camera* camera, float ray_dir_angle, float near_clip, float portal_slice_bottom, float portal_slice_top)
    {
        if (portal_slice_bottom > canvas->height - 1 || portal_slice_top < 0) return;

        int wall_index = -1;
        float ray_intersection_x, ray_intersection_y, sqr_ray_intersection_distance = std::numeric_limits<float>::max();

        for (int i = 0; i < (*sector).wall_count; i++) {
            float start_x, start_y;
            rotate(sector->walls[i]->start->x - camera->x, sector->walls[i]->start->y - camera->y, camera->heading, &start_x, &start_y);

            float end_x, end_y;
            rotate(sector->walls[i]->end->x - camera->x, sector->walls[i]->end->y - camera->y, camera->heading, &end_x, &end_y);

            if (backface_culling(start_x, start_y, end_x, end_y)) continue;

            float x, y, d;
            if (Misc::rayIntersection(start_x, start_y, end_x, end_y, ray_dir_angle, &x, &y)) {
                d = x * x + y * y;
                if (d < sqr_ray_intersection_distance) {
                    wall_index = i;
                    ray_intersection_x = x;
                    ray_intersection_y = y;
                    sqr_ray_intersection_distance = d;
                }
            }
        }

        if (wall_index != -1) {
            float horizont_line = (canvas->height >> 1) + camera->pitch;

            float ray_intersection_distance = sqrt(sqr_ray_intersection_distance);
            float slice_distance = ray_intersection_distance * cos(ray_dir_angle);

            float wall_slice_bottom = horizont_line - (camera->height - sector->bottom) / slice_distance * near_clip;
            float wall_slice_top = horizont_line + (sector->top - camera->height) / slice_distance * near_clip;

            if (sector->walls[wall_index]->portal_to == -1) {
                render_wall_slice(canvas, col, sector->walls[wall_index], camera, sector->top - sector->bottom, wall_slice_bottom, wall_slice_top, portal_slice_bottom, portal_slice_top, ray_intersection_x, ray_intersection_y);//, ray_intersection_distance);
            } else {
                float portal_bottom = wall_slice_bottom;
                float bottom_wall_height = world->sectors[sector->walls[wall_index]->portal_to].bottom - sector->bottom;
                if (bottom_wall_height > 0) {
                    portal_bottom = horizont_line - (camera->height - world->sectors[sector->walls[wall_index]->portal_to].bottom) / slice_distance * near_clip;
                    render_wall_slice(canvas, col, sector->walls[wall_index], camera, bottom_wall_height, wall_slice_bottom, portal_bottom, portal_slice_bottom, portal_slice_top, ray_intersection_x, ray_intersection_y);//, ray_intersection_distance);
                }
                portal_bottom = portal_bottom > portal_slice_bottom ? portal_bottom : portal_slice_bottom;

                float portal_top = wall_slice_top;
                float top_wall_height = sector->top - world->sectors[sector->walls[wall_index]->portal_to].top;
                if (top_wall_height > 0) {
                    portal_top = horizont_line + (world->sectors[sector->walls[wall_index]->portal_to].top - camera->height) / slice_distance * near_clip;
                    render_wall_slice(canvas, col, sector->walls[wall_index], camera, top_wall_height, portal_top, wall_slice_top, portal_slice_bottom, portal_slice_top, ray_intersection_x, ray_intersection_y);//, ray_intersection_distance);
                }
                portal_top = portal_top < portal_slice_top ? portal_top : portal_slice_top;

                render_column(canvas, col, world, &world->sectors[sector->walls[wall_index]->portal_to], camera, ray_dir_angle, near_clip, portal_bottom, portal_top);
            }

            render_floor(canvas, col, sector, camera, ray_dir_angle, near_clip, portal_slice_bottom, portal_slice_top, wall_slice_bottom);

            render_ceiling(canvas, col, sector, camera, ray_dir_angle, near_clip, portal_slice_bottom, portal_slice_top, wall_slice_top);
        }
    }

    void render(Canvas* canvas, World* world, Camera* camera, float fov)
    {
        float near_clip = (canvas->width >> 1) / tan(fov * .5f);

        // get current sector
        int curr_sector = -1;
        for (int i = 0; i < world->sector_count; i++) {
            if (world->sectors[i].pointInSector(camera->x, camera->y)) {
                curr_sector = i;
                break;
            }
        }

        if (curr_sector == -1) return;

        float ray_dir_angle = -fov * .5f;
        float ray_dir_angle_step = fov / canvas->width;
        for (int col = 0; col < canvas->width; col++) {
            render_column(canvas, col, world, &world->sectors[curr_sector], camera, ray_dir_angle, near_clip, 0, canvas->height);
            ray_dir_angle += ray_dir_angle_step;
        }
    }
}
