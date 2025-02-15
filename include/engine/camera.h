#pragma once

#include <open_gl/mouse.h>
#include <scene/point.h>
#include <misc.h>

struct Camera
{
    float x, y, height;
    float heading, pitch;

    float max_pitch = 200;
    float pitch_speed = 200;

    Camera(float x, float y, float height, float heading, float pitch)
    {
        this->x = x;
        this->y = y;
        this->height = height;
        this->heading = heading;
        this->pitch = pitch;
    }

    void update(OpenGL* openGL, Mouse* mouse, float moveSpeed)
    {
        if (glfwGetKey(openGL->window, GLFW_KEY_W) == GLFW_PRESS) {
            this->x += sin(this->heading) * moveSpeed * openGL->deltaTime;
            this->y += cos(this->heading) * moveSpeed * openGL->deltaTime;
        }
        if (glfwGetKey(openGL->window, GLFW_KEY_S) == GLFW_PRESS) {
            this->x -= sin(this->heading) * moveSpeed * openGL->deltaTime;
            this->y -= cos(this->heading) * moveSpeed * openGL->deltaTime;
        }
        if (glfwGetKey(openGL->window, GLFW_KEY_A) == GLFW_PRESS) {
            this->x -= cos(this->heading) * moveSpeed * openGL->deltaTime;
            this->y += sin(this->heading) * moveSpeed * openGL->deltaTime;
        }
        if (glfwGetKey(openGL->window, GLFW_KEY_D) == GLFW_PRESS) {
            this->x += cos(this->heading) * moveSpeed * openGL->deltaTime;
            this->y -= sin(this->heading) * moveSpeed * openGL->deltaTime;
        }
        if (glfwGetKey(openGL->window, GLFW_KEY_Q) == GLFW_PRESS) {
            this->height -= moveSpeed * openGL->deltaTime;
        }
        if (glfwGetKey(openGL->window, GLFW_KEY_E) == GLFW_PRESS) {
            this->height += moveSpeed * openGL->deltaTime;
        }

        this->heading += mouse->xDelta * mouse->mouseSensivity * openGL->deltaTime;

        this->pitch += mouse->yDelta * mouse->mouseSensivity * openGL->deltaTime * pitch_speed;
        if (this->pitch < -max_pitch) this->pitch = -max_pitch;
        if (this->pitch > max_pitch) this->pitch = max_pitch;
    }
};
