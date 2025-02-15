#include "map.h"

Map::Map()
{
    active = false;
    zoom = 1;
    zoomStepping = 0.1f;
}

void Map::changeZoom(double yScroll)
{
    if (!active) return;

    zoom += yScroll * zoomStepping;
    if (zoom < zoomStepping) zoom = zoomStepping;
}

void Map::render(Canvas* canvas, World* world, Camera* camera, bool transformed)
{
    for (Wall wall : world->walls) {
        // float start_x, start_y;
        // rotate(
        //     (*wall.start).x - camera->pos.x,
        //     (*wall.start).y - camera->pos.y,
        //     camera->heading,
        //     &start_x, &start_y
        // );
        // start_x *= zoom;
        // start_y *= zoom;
        // start_x += canvas->width >> 1;
        // start_y += canvas->height >> 1;

        Point start = wall.start->subtract(camera->pos).rotate(camera->heading).multiply(zoom).add(Point(canvas->width >> 1, canvas->height >> 1));
        Point end = wall.end->subtract(camera->pos).rotate(camera->heading).multiply(zoom).add(Point(canvas->width >> 1, canvas->height >> 1));

        // float end_x, end_y;
        // rotate(
        //     (*wall.end).x - camera->x,
        //     (*wall.end).y - camera->y,
        //     camera->heading,
        //     &end_x, &end_y
        // );
        // end_x *= zoom;
        // end_y *= zoom;
        // end_x += canvas->width >> 1;
        // end_y += canvas->height >> 1;

        Line::draw(canvas, start.x, start.y, end.x, end.y, Color(0, 255, 255));

        // wall's normal

        // float dx = end_x - start_x;
        // float dy = end_y - start_y;
        Point diff = end.subtract(start);
        float wall_length = sqrt(diff.x * diff.x + diff.y * diff.y);

        Point center = start.add(end).multiply(.5f);

        // float center_x = (start_x + end_x) * .5f;
        // float center_y = (start_y + end_y) * .5f;
        // float normal_dir_x = (dy / wall_length) * 5 * zoom;
        // float normal_dir_y = (dx / wall_length) * -5 * zoom;

        Point normal = diff.divide(wall_length).multiply(zoom, -zoom).multiply(5);

        Line::draw(canvas, center.x, center.y, center.x + normal.x, center.y + normal.y, Color(0, 255, 255));
    }
}

void Map::render_camera(Canvas* canvas, World* world, Camera* camera, float fov, bool transformed)
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
