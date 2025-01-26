#pragma once

#include <open_gl/mouse.h>
#include <scene/point.h>
#include <misc.h>

struct Camera
{
    Point pos;
    float height, heading, pitch;

    Camera(Point pos, float height, float heading, float pitch)
    {
        this->pos = pos;
        this->height = height;
        this->heading = heading;
        this->pitch = pitch;
    }

    void update(OpenGL* openGL, Mouse* mouse, float moveSpeed)
    {
        if (glfwGetKey(openGL->window, GLFW_KEY_W) == GLFW_PRESS) {
            this->pos = this->pos.add(Point(sin(this->heading), cos(this->heading)).mult(moveSpeed * openGL->deltaTime));
        }
        if (glfwGetKey(openGL->window, GLFW_KEY_S) == GLFW_PRESS) {
            this->pos = this->pos.sub(Point(sin(this->heading), cos(this->heading)).mult(moveSpeed * openGL->deltaTime));
        }
        if (glfwGetKey(openGL->window, GLFW_KEY_A) == GLFW_PRESS) {
            this->pos = this->pos.sub(Point(cos(this->heading), -sin(this->heading)).mult(moveSpeed * openGL->deltaTime));
        }
        if (glfwGetKey(openGL->window, GLFW_KEY_D) == GLFW_PRESS) {
            this->pos = this->pos.add(Point(cos(this->heading), -sin(this->heading)).mult(moveSpeed * openGL->deltaTime));
        }
        if (glfwGetKey(openGL->window, GLFW_KEY_Q) == GLFW_PRESS) {
            this->height -= moveSpeed * openGL->deltaTime;
        }
        if (glfwGetKey(openGL->window, GLFW_KEY_E) == GLFW_PRESS) {
            this->height += moveSpeed * openGL->deltaTime;
        }

        this->heading += mouse->xDelta * mouse->mouseSensivity * openGL->deltaTime;

        this->pitch += mouse->yDelta * mouse->mouseSensivity * openGL->deltaTime * 200;
        if (this->pitch < -200) this->pitch = -200;
        if (this->pitch > 200) this->pitch = 200;
    }
};
