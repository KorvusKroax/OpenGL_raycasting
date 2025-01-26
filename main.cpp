#include <open_gl.h>
#include <open_gl/mouse.h>
#include <canvas/canvas.h>
#include <scene/world.h>
#include <engine/camera.h>
#include <engine/view.h>
#include <engine/map.h>

const int WIDTH = 540; //640; //720; //800; //1024;
const int HEIGHT = 300; //480; //405; //600; //768;
const float PIXEL_SIZE = 2.5f;

Canvas canvas = Canvas(WIDTH, HEIGHT);
OpenGL openGL = OpenGL(&canvas, PIXEL_SIZE, WINDOWED);

World world;

Mouse mouse = Mouse(&openGL, 1, MOUSE_DISABLED);

Camera camera = Camera(Point(0, 0), 50, 0, 0);
float fov = Misc::deg2rad(80);
float moveSpeed = 100;

bool showMap = false;

void keys_callback(GLFWwindow* window, int key, int scancode, int action, int mod)
{
    if (key == GLFW_KEY_TAB && action == GLFW_PRESS) {
        showMap = !showMap;
    }
    if (key == GLFW_KEY_ESCAPE) {// && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(openGL.window, true);
    }
}

int main()
{
    glfwSetWindowPos(openGL.window, 100, 50);
    glfwSetKeyCallback(openGL.window, keys_callback);

    while (!glfwWindowShouldClose(openGL.window))
    {
        canvas.clearCanvas();

        View::Raycast::render(&canvas, &world, &camera, fov);

        if (showMap) {
            Map::render(&canvas, &world, &camera, true);
            Map::render_camera(&canvas, &world, &camera, fov, false);
            // Map::render_transformed_sight(&canvas, &world, &camera);
        }

        mouse.update(&openGL);
        camera.update(&openGL, &mouse, moveSpeed);
        openGL.update(&canvas);
    }

    return 0;
}
