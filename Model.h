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
    Model(const char* filepath)
    {
        this->loadModel(filepath);
    }

    ~Model()
    {

    }

    void Draw(Shader& shader)
    {
        for (size_t i = 0; i < meshes.size(); i++)
        {
            meshes[i].Draw(shader);
        }
    }

private:
    std::vector<Mesh> meshes;
    std::vector<Texture> texLoaded;

    std::string directory;

    void loadModel(std::string filepath);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);

    std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
};

#endif
