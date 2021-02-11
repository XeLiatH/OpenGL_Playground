#pragma once

#ifndef TEXTURE_H
#define TEXTURE_H

#include <iostream>

#include <opencv2\opencv.hpp>

#include <GL/glew.h> 
#include <GL/wglew.h>
#include <GLFW/glfw3.h>

class Texture
{
public:
    Texture(std::string filePath, bool transparent = false);
    ~Texture();

    void bind();

private:
    unsigned int id;

    void loadFromFile(std::string filePath, bool transparent);

};


#endif
