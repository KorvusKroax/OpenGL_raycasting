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

const float zoomStepping = 0.1f;
float zoom = 1;
void scroll_callback(GLFWwindow* window, double xOffset, double yOffset)
{
    zoom += yOffset * zoomStepping;
    if (zoom < zoomStepping) zoom = zoomStepping;
}

int main()
{
    Canvas canvas = Canvas(WIDTH, HEIGHT);
    OpenGL openGL = OpenGL(&canvas, PIXEL_SIZE, WINDOWED);

    World world;

    Viewpoint viewpoint = Viewpoint(Point(0, 0), 25, 0);
    float moveSpeed = 50;

    Control::Mouse::init(&openGL, scroll_callback, Control::Mouse::MOUSE_DISABLED);
    Control::Keys::init();
    double mouseSensivity = 1;

    while (!glfwWindowShouldClose(openGL.window))
    {
        canvas.clearCanvas();

        // Map::render(&canvas, &world, &viewpoint, zoom);
        // Map::render_viewpoint(&canvas, &world, &viewpoint, zoom, 80);

        Map::render_transformed(&canvas, &world, &viewpoint, zoom);
        Map::render_transformed_sight(&canvas, &world, &viewpoint, zoom);
        Map::render_transformed_viewpoint(&canvas, &world, &viewpoint, zoom, 80);

        Sight::render(&canvas, &world, &viewpoint, 80);

        Control::Keys::update(&openGL);
        Control::Mouse::update(&openGL);

        viewpoint.update(
            Control::Keys::xStep * moveSpeed * openGL.deltaTime,
            Control::Keys::yStep * moveSpeed * openGL.deltaTime,
            Control::Mouse::xDelta * mouseSensivity * openGL.deltaTime
        );

        if (glfwGetKey(openGL.window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(openGL.window, true);
        }

        openGL.update(&canvas);
    }

    return 0;
}
