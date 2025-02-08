#include <open_gl.h>
#include <open_gl/mouse.h>
#include <canvas/canvas.h>
#include <scene/world.h>
#include <engine/camera.h>
#include <engine/view.h>
#include <engine/map.h>

const int WIDTH = 320;
const int HEIGHT = 200;
const float PIXEL_SIZE = 4.0f;

Canvas canvas = Canvas(WIDTH, HEIGHT);
OpenGL openGL = OpenGL(&canvas, PIXEL_SIZE, WINDOWED);

World world;

Mouse mouse = Mouse(&openGL, .5f, MOUSE_CURSOR_DISABLED);

Camera camera = Camera(0, 0, 50, 0, 0);
float fov = Misc::deg2rad(80);
float moveSpeed = 50;

int main()
{
    glfwSetWindowPos(openGL.window, 100, 50);
    glfwSetKeyCallback(openGL.window,
        [](GLFWwindow* window, int key, int scancode, int action, int mod) {
            if (key == GLFW_KEY_TAB && action == GLFW_PRESS) {
                Map::map_is_active = !Map::map_is_active;
            }
            if (key == GLFW_KEY_ESCAPE) {// && action == GLFW_PRESS) {
                glfwSetWindowShouldClose(openGL.window, true);
            }
        }
    );

    while (!glfwWindowShouldClose(openGL.window)) {
        canvas.clearCanvas();
        // canvas.fillCanvas(Color(255, 0, 255));

        View::render(&canvas, &world, &camera, fov);

        if (Map::map_is_active) {
            Map::changeZoom(mouse.yScroll);
            Map::render(&canvas, &world, &camera, true);
            Map::render_camera(&canvas, &world, &camera, fov, false);
        }

        mouse.update(&openGL);
        camera.update(&openGL, &mouse, moveSpeed);
        openGL.update(&canvas);
    }

    return 0;
}
