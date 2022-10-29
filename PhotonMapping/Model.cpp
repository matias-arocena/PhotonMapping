#include "Model.h"
#include "Mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>
#include "Material.h"
#include "Device.h"

Model::Model(const char* objFilePath, glm::vec3 position)
{
    this->position = position;
    this->meshes = std::vector<std::shared_ptr<Mesh>>();
    this->loadModel(objFilePath);
}

void Model::loadModel(const char* objFilePath)
{
    Assimp::Importer import;

    std::cout << objFilePath << std::endl;
    const aiScene* scene = import.ReadFile(objFilePath, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_FixInfacingNormals);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
        return;
    }

    // Process materials
    for (unsigned int i = 0; i < scene->mNumMaterials; i++)
    {
        aiMaterial* material = scene->mMaterials[i];
        this->materials.push_back(std::make_shared<Material>(material));
    }

    processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
    // Process meshes
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        this->meshes.push_back(createMesh(mesh, scene));
    }

    // then do the same for each of its children
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }
}

std::vector<std::shared_ptr<Mesh>> Model::getMeshes()
{
    return this->meshes;
}

std::shared_ptr<Mesh> Model::createMesh(aiMesh* mesh, const aiScene* scene)
{
#ifdef _DEBUG
    std::cout << "Creating Mesh" << std::endl;
#endif

    RTCGeometry geometry = rtcNewGeometry(Device::getInstance().getDevice(), RTC_GEOMETRY_TYPE_TRIANGLE);

    float * vertexBuffer   = (float*)rtcSetNewGeometryBuffer(geometry, RTC_BUFFER_TYPE_VERTEX, 0, RTC_FORMAT_FLOAT3, 3 * sizeof(float), mesh->mNumVertices);

    unsigned * indexBuffer = (unsigned*)rtcSetNewGeometryBuffer(geometry, RTC_BUFFER_TYPE_INDEX, 0, RTC_FORMAT_UINT3, 3 * sizeof(unsigned), mesh->mNumFaces);
   
    RTCError e3 = rtcGetDeviceError(Device::getInstance().getDevice());

#pragma omp parallel for
    for (int i = 0; i < mesh->mNumVertices; i++)
    {
        // Vertex
        vertexBuffer[i * 3]     = mesh->mVertices[i].x + this->position.x;
        vertexBuffer[i * 3 + 1] = mesh->mVertices[i].y + this->position.y;
        vertexBuffer[i * 3 + 2] = mesh->mVertices[i].z + this->position.z;

    }

#pragma omp parallel for
    // Asumo que mesh siempre tiene triángulos
    for (int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        indexBuffer[i * 3] = face.mIndices[0];
        indexBuffer[i * 3 + 1] = face.mIndices[1];
        indexBuffer[i * 3 + 2] = face.mIndices[2];
    }
    rtcCommitGeometry(geometry);

    std::shared_ptr<Material> material = this->materials[mesh->mMaterialIndex];
    
    return std::make_shared<Mesh>(vertexBuffer, indexBuffer, geometry, material);
}