#define STB_IMAGE_IMPLEMENTATION

#include <canvas.h>
#include <open_gl.h>
#include <mouse.h>

#include "src/scene/point.h"
#include "src/scene/world.h"
#include "src/camera.h"
#include "src/map.h"

const double PI = 3.141592653589793238463;

const int WIDTH = 320;
const int HEIGHT = 200;
const float PIXEL_SIZE = 4.0f;

Canvas canvas = Canvas(WIDTH, HEIGHT);
OpenGL openGL = OpenGL(&canvas, PIXEL_SIZE, WINDOWED);
Mouse mouse = Mouse(&openGL, .5f, MOUSE_CURSOR_DISABLED);

World world;
Map map;
float fov = 80;
Camera camera = Camera(Point(0, 0), 50, 0, 0, fov / 180.0 * PI);

void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mod)
{
    if (key == GLFW_KEY_TAB && action == GLFW_PRESS) {
        map.active = !map.active;
    }
    if (key == GLFW_KEY_ESCAPE) {
        glfwSetWindowShouldClose(openGL.window, true);
    }
}

int main()
{
    glfwSetWindowPos(openGL.window, 100, 50);
    glfwSetKeyCallback(openGL.window, keyCallback);

    while (!glfwWindowShouldClose(openGL.window)) {
        canvas.clearCanvas();

        mouse.update(&openGL);
        camera.update(&openGL, &mouse);
        camera.render(&canvas, &world);
        if (map.active) {
            map.changeZoom(mouse.yScroll);
            map.render(&canvas, &world, &camera);
            map.render_viewpoint(&canvas, &world, &camera);
        }

        openGL.update(&canvas);
    }

    return EXIT_SUCCESS;
}
