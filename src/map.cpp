#include "map.h"

void Map::render(Canvas* canvas, World* world, Camera* camera)
{
    for (Wall wall : world->walls) {
        Point start = wall.start->subtract(camera->pos).rotate(camera->heading).multiply(zoom).add(Point(canvas->width >> 1, canvas->height >> 1));
        Point end = wall.end->subtract(camera->pos).rotate(camera->heading).multiply(zoom).add(Point(canvas->width >> 1, canvas->height >> 1));

        canvas->drawLine(start.x, start.y, end.x, end.y, Color(0, 255, 255));

        // wall's normal
        Point diff = end.subtract(start);
        float wallLength = sqrt(diff.x * diff.x + diff.y * diff.y);

        Point center = start.add(end).multiply(.5f);

        float normalLength = 5;
        Point normal = Point(diff.y, -diff.x).divide(wallLength).multiply(zoom * normalLength);

        canvas->drawLine(center.x, center.y, center.x + normal.x, center.y + normal.y, Color(0, 255, 255));
    }
}

void Map::render_viewpoint(Canvas* canvas, World* world, Camera* camera)
{
    float dirLength = 10;
    canvas->drawLine((canvas->width >> 1), (canvas->height >> 1), (canvas->width >> 1), (canvas->height >> 1) + dirLength, Color(255, 127, 0));
    canvas->drawCircle_filled((canvas->width >> 1), (canvas->height >> 1), 2 * zoom, Color(255, 127, 0));

    // fov lines
    float fovLength = 100;
    float fov_angle = camera->fov * .5f;
    Point fov_leftSide = Point(sin(-fov_angle), cos(-fov_angle)).multiply(zoom * fovLength);
    Point fov_rightSide = Point(sin(fov_angle), cos(fov_angle)).multiply(zoom * fovLength);

    canvas->drawLine((canvas->width >> 1), (canvas->height >> 1), (canvas->width >> 1) + fov_leftSide.x, (canvas->height >> 1) + fov_leftSide.y, Color(255, 127, 0, 127));
    canvas->drawLine((canvas->width >> 1), (canvas->height >> 1), (canvas->width >> 1) + fov_rightSide.x, (canvas->height >> 1) + fov_rightSide.y, Color(255, 127, 0, 127));
}

void Map::changeZoom(double yScroll)
{
    if (!active) return;

    zoom += yScroll * zoomStepping;
    if (zoom < zoomStepping) zoom = zoomStepping;
}
