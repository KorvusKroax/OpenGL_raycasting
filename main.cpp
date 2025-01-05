#include <open_gl.h>
#include <canvas/canvas.h>
#include <engine/world.h>
#include <engine/viewpoint.h>
#include <engine/map.h>
#include <engine/sight.h>
#include <engine/control.h>

const int WIDTH = 540; //640; //720; //800; //1024;
const int HEIGHT = 300; //480; //405; //600; //768;
const float PIXEL_SIZE = 3;

const double mouseSensivity = 1;
float moveSpeed = 50;

int main()
{
    Canvas canvas = Canvas(WIDTH, HEIGHT);
    OpenGL openGL = OpenGL(&canvas, PIXEL_SIZE, WINDOWED);

    World world;

    Viewpoint viewpoint = Viewpoint(Point(0, 0), 25, 0);
    float moveSpeed = 50;

    Control::Mouse::init(&openGL, Map::zoom_callback, Control::Mouse::MOUSE_DISABLED);
    Control::Keys::init();

    while (!glfwWindowShouldClose(openGL.window))
    {
        canvas.clearCanvas();

        Sight::render_raycast(&canvas, &world, &viewpoint, 80);
        Sight::render_wireframe(&canvas, &world, &viewpoint, 80);

        Map::render(&canvas, &world, &viewpoint);
        Map::render_viewpoint(&canvas, &world, &viewpoint, 80);

        // Map::render_transformed(&canvas, &world, &viewpoint);
        // Map::render_transformed_sight(&canvas, &world, &viewpoint);
        // Map::render_transformed_viewpoint(&canvas, &world, &viewpoint, 80);

        Control::Keys::update(&openGL);
        Control::Mouse::update(&openGL);

        viewpoint.update(
            Control::Keys::xStep * moveSpeed * openGL.deltaTime,
            Control::Keys::yStep * moveSpeed * openGL.deltaTime,
            Control::Keys::rise * moveSpeed * openGL.deltaTime,
            Control::Mouse::xDelta * mouseSensivity * openGL.deltaTime
        );

        if (glfwGetKey(openGL.window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(openGL.window, true);
        }

        openGL.update(&canvas);
    }

    return 0;
}
