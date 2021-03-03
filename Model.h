#pragma once

#ifndef MODEL_H
#define MODEL_H

#include <iostream>
#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <opencv2\opencv.hpp>

#include "Shader.h";
#include "Mesh.h";

unsigned int loadTextureFromFile(const char* name, const std::string& directory);

class Model
{
public:
    Model(const char* filepath, Shader& shader)
        :shader(shader)
    {
        this->loadModel(filepath);
    }

    ~Model()
    {

    }

    void Translate(glm::vec3 position)
    {
        this->shader.use();
        model = glm::translate(model, position);

        this->shader.setMat4("model", model);
    }

    void Rotate(float angle, glm::vec3 direction)
    {
        this->shader.use();
        model = glm::rotate(model, glm::radians(angle), direction);

        this->shader.setMat4("model", model);
    }

    void Scale(glm::vec3 scale)
    {
        this->shader.use();
        model = glm::scale(model, scale);

        this->shader.setMat4("model", model);
    }

    void Draw()
    {
        for (size_t i = 0; i < meshes.size(); i++)
        {
            meshes[i].Draw(this->shader);
        }

        this->model = glm::mat4(1.f);
    }

private:
    Shader& shader;

    glm::mat4 model = glm::mat4(1.f);

    std::vector<Mesh> meshes;
    std::vector<Texture> texLoaded;

    std::string directory;

    void loadModel(std::string filepath);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);

    std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
};

#endif
