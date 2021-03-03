#pragma once

#ifndef FLASHLIGHT_H
#define FLASHLIGHT_H

#include <GL/glew.h> 
#include <GL/wglew.h>

#include "Shader.h";

class Flashlight
{
public:
    Flashlight(glm::vec3 position, glm::vec3 direction)
    {
        this->position = position;
        this->direction = direction;
    }

    ~Flashlight()
    {

    }

    void Render(Shader& shader);

    void SetPosition(glm::vec3 position)
    {
        this->position = position;
    }

    void SetDirection(glm::vec3 direction)
    {
        this->direction = direction;
    }
private:
    glm::vec3 position;
    glm::vec3 direction;
};

#endif
