#pragma once

#include <canvas.h>
#include <line.h>
#include <circle.h>

#include "scene/wall.h"
#include "scene/world.h"
#include "camera.h"

class Map
{
    public:
        bool active;
        float zoom;
        float zoomStepping;

        Map();

        void changeZoom(double yScroll);
        void render(Canvas* canvas, World* world, Camera* camera, bool transformed = false);
        void render_camera(Canvas* canvas, World* world, Camera* camera, float fov, bool transformed = true);
};
