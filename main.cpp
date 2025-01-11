#include <open_gl.h>
#include <canvas/canvas.h>
#include <engine/world.h>
#include <engine/viewpoint.h>
#include <engine/map.h>
#include <engine/sight.h>
#include <engine/control.h>

const int WIDTH = 540; //640; //720; //800; //1024;
const int HEIGHT = 300; //480; //405; //600; //768;
const float PIXEL_SIZE = 2.5f;

const double mouseSensivity = 1;
float moveSpeed = 100;

float fov = Misc::deg2rad(80);

int main()
{
    Canvas canvas = Canvas(WIDTH, HEIGHT);

    OpenGL openGL = OpenGL(&canvas, PIXEL_SIZE, WINDOWED);
    glfwSetWindowPos(openGL.window, 100, 50);

    World world;

    Viewpoint viewpoint = Viewpoint(Point(0, 0), 50, 0, 0);
    float moveSpeed = 50;

    Control::Mouse::init(&openGL, Map::zoom_callback, Control::Mouse::MOUSE_DISABLED);
    Control::Keys::init();

    while (!glfwWindowShouldClose(openGL.window))
    {
        canvas.clearCanvas();

        // Line::draw(&canvas, 0, canvas.height >> 1, canvas.width - 1, canvas.height >> 1, Color(255, 0, 0));

        Sight::render_raycast(&canvas, &world, &viewpoint, fov);
        // Sight::render_wireframe(&canvas, &world, &viewpoint, fov);

        Map::render(&canvas, &world, &viewpoint, true);
        Map::render_viewpoint(&canvas, &world, &viewpoint, fov, false);
        Map::render_transformed_sight(&canvas, &world, &viewpoint);

        Control::Keys::update(&openGL);
        Control::Mouse::update(&openGL);

        viewpoint.update(
            Control::Keys::xStep * moveSpeed * openGL.deltaTime,
            Control::Keys::yStep * moveSpeed * openGL.deltaTime,
            Control::Keys::rise * moveSpeed * openGL.deltaTime,
            Control::Mouse::xDelta * mouseSensivity * openGL.deltaTime,
            Control::Mouse::yDelta * mouseSensivity * 100 * openGL.deltaTime
        );

        std::cout << Control::Mouse::yDelta * mouseSensivity * openGL.deltaTime << std::endl;

        if (glfwGetKey(openGL.window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(openGL.window, true);
        }

        openGL.update(&canvas);
    }

    return 0;
}
