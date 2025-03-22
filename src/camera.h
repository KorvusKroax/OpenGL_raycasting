#pragma once

#include "canvas.h"
#include "open_gl.h"
#include "mouse.h"
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

        float moveSpeed = 100;
        float turnSpeed = 1;
        float pitchSpeed = turnSpeed * 150;
        float maxPitch = 150;

        Camera(Point pos, float height, float heading, float pitch, float fov);
        ~Camera();

        void update(OpenGL *openGL, Mouse *mouse);
        void render(Canvas *canvas, World *world);

    private:
        void render_column(Canvas *canvas, int col, World *world, Sector *sector, float rayAngle, float nearClip, int portalSlice_bottom, int portalSlice_top);
        void render_wallSlice(Canvas *canvas, int col, World *world, Wall *wall, float wall_height, float wallSlice_bottom, float wallSlice_top, int portalSlice_bottom, int portalSlice_top, Point intersection);
        void render_floor(Canvas *canvas, int col, World *world, Sector *sector, float rayAngle, float nearClip, int portalSlice_bottom, int portalSlice_top, float wallSlice_bottom);
        void render_ceiling(Canvas *canvas, int col, World *world, Sector *sector, float rayAngle, float nearClip, int portalSlice_bottom, int portalSlice_top, float wallSlice_top);

        bool pointInSector(World *world, Sector *sector);
        bool backfaceCulling(Point *start, Point *end);
        bool rayIntersection(Point *start, Point *end, float rayAngle, Point *intersection);
        double nmod(double value, double mod);
};
