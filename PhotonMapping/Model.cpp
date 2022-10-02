#include "Model.h"
#include "Mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>

Model::Model(const char* objFilePath, RTCDevice device)
{
    this->meshes = std::vector<std::shared_ptr<Mesh>>();
    this->loadModel(objFilePath, device);
}

void Model::loadModel(const char* objFilePath, RTCDevice device)
{
    Assimp::Importer import;

    std::cout << objFilePath << std::endl;
    const aiScene* scene = import.ReadFile(objFilePath, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
        return;
    }

    processNode(scene->mRootNode, scene, device);
}

void Model::processNode(aiNode* node, const aiScene* scene, RTCDevice device)
{
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];

        Mesh newMesh = Mesh(mesh, scene, device);
        std::shared_ptr<Mesh> meshPtr = std::make_shared<Mesh>(newMesh);
        meshes.push_back(meshPtr);
    }
}

std::vector<std::shared_ptr<Mesh>> Model::getMeshes()
{
    return this->meshes;
}