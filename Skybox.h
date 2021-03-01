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
    Skybox(std::vector<std::string> faces)
    {
        glGenTextures(1, &this->id);
        glBindTexture(GL_TEXTURE_CUBE_MAP, this->id);

        cv::Mat image;
        //cv::Mat source;

        for (size_t i = 0; i < faces.size(); i++)
        {
            image = cv::imread(faces[i]);
            //cv::flip(source, image, 0);

            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, image.cols, image.rows, 0, GL_BGR, GL_UNSIGNED_BYTE, image.data);
        }

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    }

    ~Skybox()
    {

    }

    GLuint getId() const
    {
        return this->id;
    }

    void Draw(Shader& shader);
private:
    GLuint id;
};

#endif
