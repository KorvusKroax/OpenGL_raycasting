#pragma once

#include <open_gl.h>

enum MouseMode {
    MOUSE_ENABLED,
    MOUSE_DISABLED,
    MOUSE_HIDDEN
};

class Mouse
{
    public:
        double mouseSensivity;

        double xPos, yPos, xPos_last, yPos_last, xDelta, yDelta;

        // static double xScroll, yScroll;
        // static void scroll_callback(GLFWwindow* window, double xoff, double yoff)
        // {
        //     yScroll += yoff;
        //     std::cout << "yScroll: " << yScroll << std::endl;
        // }

        Mouse(OpenGL* openGL, double mouseSensivity = 1, MouseMode mouseMode = MOUSE_ENABLED)
        {
            this->mouseSensivity = mouseSensivity;

            switch (mouseMode) {
                case MOUSE_HIDDEN:
                    glfwSetInputMode(openGL->window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
                    glfwSetInputMode(openGL->window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
                    break;
                case MOUSE_DISABLED:
                    glfwSetInputMode(openGL->window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
                    glfwSetInputMode(openGL->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                    break;
            }
            glfwGetCursorPos(openGL->window, &xPos, &yPos);
            xPos_last = xPos;
            yPos_last = yPos;

            // glfwSetScrollCallback(openGL->window, scroll_callback);
        }

        void update(OpenGL* openGL)
        {
            glfwGetCursorPos(openGL->window, &xPos, &yPos);
            xDelta = xPos - xPos_last;
            yDelta = yPos - yPos_last;
            xPos_last = xPos;
            yPos_last = yPos;
        }
};
