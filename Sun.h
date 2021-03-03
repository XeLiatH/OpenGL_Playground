#pragma once

#ifndef SUN_H
#define SUN_H

#include <GL/glew.h> 
#include <GL/wglew.h>

#include "Shader.h";

class Sun
{
public:
    Sun(glm::vec3 direction = glm::vec3(-0.2f, -1.0f, -0.3f), glm::vec3 color = glm::vec3(0.98f, 0.553f, 0.125f))
    {
        this->direction = direction;
        this->color = color;
    }

    ~Sun()
    {

    }

    void Render(Shader& shader);
private:
    glm::vec3 direction;
    glm::vec3 color;

    glm::vec3 ambient = glm::vec3(0.05f, 0.05f, 0.05f);
    glm::vec3 diffuse = glm::vec3(0.4f, 0.4f, 0.4f);
    glm::vec3 specular = glm::vec3(0.5f, 0.5f, 0.5f);
};

#endif
