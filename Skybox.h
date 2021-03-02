#pragma once

#ifndef SKYBOX_H
#define SKYBOX_H

#include <opencv2/opencv.hpp>

#include <GL/glew.h> 
#include <GL/wglew.h>

#include <iostream>
#include <vector>

#include "Shader.h";

class Skybox
{
public:
    Skybox(std::vector<std::string> faces);

    ~Skybox()
    {

    }

    void Draw(Shader& shader);
private:
    GLuint id;
    GLuint VAO, VBO;

    void LoadTextures(std::vector<std::string> faces);
};

#endif
