#pragma once

#include <canvas/canvas.h>
#include <scene/world.h>
#include <misc.h>
#include "camera.h"

namespace View
{
    namespace Raycast
    {
        void render_wall_slice(Canvas* canvas, int col, Wall* wall, Camera* camera, float sector_height, float wall_slice_bottom, float wall_slice_top, Point rayIntersection)
        {
            Point t_start = (*(*wall).start).sub(camera->pos).rotate(camera->heading);
            float t_dx = rayIntersection.x - t_start.x;
            float t_dy = rayIntersection.y - t_start.y;
            float texture_slice_x = fmod(sqrt(t_dx * t_dx + t_dy * t_dy), (*(*wall).texture).width);

            float texture_step_y =  sector_height / (wall_slice_top - wall_slice_bottom);
            // float f = (int)wall_slice_bottom * texture_step_y;
            // float texture_slice_y = 1.0f - abs(f - (int)f);
            float texture_slice_y = 0;

            for (int row = wall_slice_bottom; row < wall_slice_top; row++) {
                int color_index = (int)texture_slice_x + (int)texture_slice_y * (*(*wall).texture).width;
                canvas->setPixel(col, row, (*(*wall).texture).pixels[color_index]);

                texture_slice_y += texture_step_y;
                if (texture_slice_y >= (*(*wall).texture).height) {
                    texture_slice_y -= (*(*wall).texture).height;
                }
            }
        }

        void render_floor(Canvas* canvas, int col, Sector* sector, Camera* camera, float ray_dir_angle, float near_clip, float horizont_line, float portal_slice_bottom, float wall_slice_bottom)
        {
            for (int row = portal_slice_bottom; row < wall_slice_bottom - 1; row++) {

                float t_dist = camera->height / (row - horizont_line) * near_clip / cos(ray_dir_angle);

                float t_pos_x = Misc::nmod(camera->pos.x - t_dist * sin(camera->heading + ray_dir_angle), (*(*sector).floor_texture).width);
                float t_pos_y = Misc::nmod(camera->pos.y - t_dist * cos(camera->heading + ray_dir_angle), (*(*sector).floor_texture).height);

                int color_index = (int)t_pos_x + (int)t_pos_y * (*(*sector).floor_texture).width;
                canvas->setPixel(col, row, (*(*sector).floor_texture).pixels[color_index]);
            }
        }

        void render_ceiling(Canvas* canvas, int col, Sector* sector, Camera* camera, float ray_dir_angle, float near_clip, float horizont_line, float portal_slice_top, float wall_slice_top)
        {
            for (int row = wall_slice_top + 1; row < portal_slice_top; row++) {

                float t_dist = (((*sector).top - (*sector).bottom) - camera->height) / (horizont_line - row) * near_clip / cos(ray_dir_angle);

                float t_pos_x = Misc::nmod(camera->pos.x - t_dist * sin(camera->heading + ray_dir_angle), (*(*sector).ceiling_texture).width);
                float t_pos_y = Misc::nmod(camera->pos.y - t_dist * cos(camera->heading + ray_dir_angle), (*(*sector).ceiling_texture).height);

                int color_index = (int)t_pos_x + (int)t_pos_y * (*(*sector).ceiling_texture).width;
                canvas->setPixel(col, row, (*(*sector).ceiling_texture).pixels[color_index]);
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
            int wall_index = -1;
            float ix, iy, iDist = std::numeric_limits<float>::max();

            for (int i = 0; i < (*sector).wall_count; i++) {
                Point start = (*(*(*sector).walls[i]).start).sub(camera->pos).rotate(camera->heading);
                Point end = (*(*(*sector).walls[i]).end).sub(camera->pos).rotate(camera->heading);

                if (backface_culling(start.x, start.y, end.x, end.y)) continue;

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
                float horizont_line = (canvas->height >> 1) + camera->pitch;

                float wall_slice_top = horizont_line + ((*sector).top - camera->height) / slice_dist * near_clip;
                float wall_slice_bottom = horizont_line - (camera->height - (*sector).bottom) / slice_dist * near_clip;

                if ((*(*sector).walls[wall_index]).portal_to != -1) {
                    render_column(canvas, col, world, &world->sectors[(*(*sector).walls[wall_index]).portal_to], camera, ray_dir_angle, near_clip, wall_slice_bottom, wall_slice_top);
                } else {
                    render_wall_slice(canvas, col, (*sector).walls[wall_index], camera, (*sector).top - (*sector).bottom, wall_slice_bottom, wall_slice_top, Point(ix, iy));
                }

                // render_floor(canvas, col, sector, camera, ray_dir_angle, near_clip, horizont_line, portal_slice_bottom, wall_slice_bottom);

                // render_ceiling(canvas, col, sector, camera, ray_dir_angle, near_clip, horizont_line, portal_slice_top, wall_slice_top);
            }
        }

        void render(Canvas* canvas, World* world, Camera* camera, float fov)
        {
            float near_clip = (canvas->width >> 1) / tan(fov * .5f);

            // get current sector
            int curr_sector = -1;
            for (int i = 0; i < world->sector_count; i++) {
                if (world->sectors[i].pointInSector(camera->pos.x, camera->pos.y)) {
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
}
