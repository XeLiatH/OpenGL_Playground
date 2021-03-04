#pragma once

#ifndef POINTLIGHT_H
#define POINTLIGHT_H

#include <GL/glew.h> 
#include <GL/wglew.h>

#include "Shader.h";

class Pointlight
{
public:
    Pointlight(Shader& shader)
        :shader(shader)
    {

    }

    ~Pointlight()
    {

    }

    void setPosition(glm::vec3 position)
    {
        this->position = position;
    }

    void Render(int i);

private:
    Shader& shader;

    glm::vec3 position;

    glm::vec3 ambient = glm::vec3(0.05f, 0.05f, 0.05f);
    glm::vec3 diffuse = glm::vec3(0.8f, 0.8f, 0.8f);
    glm::vec3 specular = glm::vec3(1.0f, 1.0f, 1.0f);

    float constant = 1.f;
    float linear = 0.09f;
    float quadratic = 0.032;
};

#endif
