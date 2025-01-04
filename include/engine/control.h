#pragma once

#include <open_gl.h>

namespace Control
{
    namespace Mouse
    {
        enum Mode {
            MOUSE_ENABLED,
            MOUSE_DISABLED,
            MOUSE_HIDDEN
        };

        double xPos, yPos, xPos_last, yPos_last;
        double xDelta, yDelta;

        void init(OpenGL* openGL, GLFWscrollfun scroll_callback, Mode mouseMode = MOUSE_ENABLED)
        {
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

            glfwSetScrollCallback(openGL->window, scroll_callback);
        }

        void update(OpenGL* openGL)
        {
            glfwGetCursorPos(openGL->window, &xPos, &yPos);
            xDelta = xPos - xPos_last;
            yDelta = yPos - yPos_last;
            xPos_last = xPos;
            yPos_last = yPos;
        }
    }

    namespace Keys
    {
        int xStep, yStep, rise;

        void init()
        {
            xStep = 0;
            yStep = 0;
            rise = 0;
        }

        void update(OpenGL* openGL)
        {
            xStep = 0;
            yStep = 0;
            rise = 0;
            if (glfwGetKey(openGL->window, GLFW_KEY_W) == GLFW_PRESS) yStep = +1;
            if (glfwGetKey(openGL->window, GLFW_KEY_D) == GLFW_PRESS) xStep = +1;
            if (glfwGetKey(openGL->window, GLFW_KEY_S) == GLFW_PRESS) yStep = -1;
            if (glfwGetKey(openGL->window, GLFW_KEY_A) == GLFW_PRESS) xStep = -1;
            if (glfwGetKey(openGL->window, GLFW_KEY_Q) == GLFW_PRESS) rise = -1;
            if (glfwGetKey(openGL->window, GLFW_KEY_E) == GLFW_PRESS) rise = +1;
        }
    }
}
