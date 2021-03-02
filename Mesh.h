#pragma once

#ifndef MESH_H
#define MESH_H

#include <iostream>
#include <vector>

#include "Shader.h";

struct Vertex
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texture;
};

struct Texture {
    GLuint id;
    std::string type;
    std::string path;
};

class Mesh
{
public:
    Mesh(const std::vector<Vertex>& vertices, const std::vector<GLuint>& indices, const std::vector<Texture>& textures)
    {
        this->vertices = vertices;
        this->indices = indices;
        this->textures = textures;

        this->setup();
    }

    ~Mesh()
    {

    }

    void Draw(Shader& shader);

    const std::vector<Vertex>& getVertices() const
    {
        return this->vertices;
    }

    const std::vector<GLuint>& getIndices() const
    {
        return this->indices;
    }

    const std::vector<Texture>& getTextures() const
    {
        return this->textures;
    }

private:
    unsigned int VAO, VBO, EBO;

    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    std::vector<Texture> textures;

    void setup();
};

#endif
