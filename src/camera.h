#pragma once

#include <canvas.h>
#include <open_gl.h>
#include <mouse.h>

#include "scene/point.h"
#include "scene/wall.h"
#include "scene/sector.h"
#include "scene/world.h"

class Camera
{
    public:
        Point pos;
        float height;
        float heading, pitch;
        float fov;

        Camera(Point pos, float height, float heading, float pitch, float fov);
        ~Camera();

        void update(OpenGL* openGL, Mouse* mouse);
        void render(Canvas* canvas, World* world);

    private:
        float move_speed;
        float max_pitch, pitch_speed;

        void render_column(Canvas* canvas, int col, World* world, Sector* sector, float ray_dir_angle, float near_clip, float portal_slice_bottom, float portal_slice_top);
        void render_wall_slice(Canvas* canvas, int col, Wall* wall, float wall_height, float wall_slice_bottom, float wall_slice_top, float portal_bottom, float portal_top, Point* intersection);
        void render_floor(Canvas* canvas, int col, Sector* sector, float ray_dir_angle, float near_clip, float portal_slice_bottom, float portal_slice_top, float wall_slice_bottom);
        void render_ceiling(Canvas* canvas, int col, Sector* sector, float ray_dir_angle, float near_clip, float portal_slice_bottom, float portal_slice_top, float wall_slice_top);

        bool ray_intersection(Point* start, Point* end, float ray_dir_angle, Point* intersection);
        double nmod(double value, double mod);
};
