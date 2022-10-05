#include "Model.h"
#include "Mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>
#include "Material.h"

Model::Model(const char* objFilePath, RTCDevice device, glm::vec3 position)
{
    this->position = position;
    this->meshes = std::vector<std::shared_ptr<Mesh>>();
    this->loadModel(objFilePath, device);
}

void Model::loadModel(const char* objFilePath, RTCDevice device)
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

    processNode(scene->mRootNode, scene, device);
}

void Model::processNode(aiNode* node, const aiScene* scene, RTCDevice device)
{
    // Process meshes
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        this->meshes.push_back(createMesh(mesh, scene, device));
    }

    // then do the same for each of its children
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene, device);
    }
}

std::vector<std::shared_ptr<Mesh>> Model::getMeshes()
{
    return this->meshes;
}

std::shared_ptr<Mesh> Model::createMesh(aiMesh* mesh, const aiScene* scene, RTCDevice device)
{
#ifdef _DEBUG
    std::cout << "Creating Mesh" << std::endl;
#endif

    RTCGeometry geometry = rtcNewGeometry(device, RTC_GEOMETRY_TYPE_TRIANGLE);

    float * vertexBuffer   = (float*)rtcSetNewGeometryBuffer(geometry, RTC_BUFFER_TYPE_VERTEX, 0, RTC_FORMAT_FLOAT3, 3 * sizeof(float), mesh->mNumVertices);

    unsigned * indexBuffer = (unsigned*)rtcSetNewGeometryBuffer(geometry, RTC_BUFFER_TYPE_INDEX, 0, RTC_FORMAT_UINT3, 3 * sizeof(unsigned), mesh->mNumFaces);
   
    RTCError e3 = rtcGetDeviceError(device);

    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        // Vertex
        vertexBuffer[i * 3]     = mesh->mVertices[i].x + this->position.x;
        vertexBuffer[i * 3 + 1] = mesh->mVertices[i].y + this->position.y;
        vertexBuffer[i * 3 + 2] = mesh->mVertices[i].z + this->position.z;

    }

    // Asumo que mesh siempre tiene triángulos
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
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