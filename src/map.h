#pragma once

#include <canvas.h>
#include "scene/world.h"
#include "camera.h"

class Map
{
    public:
        bool active = false;
        float zoom = 1;
        float zoomStepping = 0.1f;

        void render(Canvas* canvas, World* world, Camera* camera);
        void render_viewpoint(Canvas* canvas, World* world, Camera* camera);
        void changeZoom(double yScroll);
};
