#pragma once

#ifndef SHADER_H
#define SHADER_H

#include <iostream>
#include <fstream>
#include <sstream>

#include <GL/glew.h> 
#include <GL/wglew.h>
#include <GLFW/glfw3.h>

class Shader
{
public:
    Shader(const char* vertexPath, const char* fragmentPath);
    ~Shader();

    void use();

    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;

private:
    unsigned int id;

    std::string loadShaderSource(std::string shaderFilePath);
    unsigned int loadShader(GLenum type, std::string shaderFilePath);
    void linkProgram(unsigned int vertexShader, unsigned int fragmentShader);
};

#endif
