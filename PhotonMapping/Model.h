#pragma once
#include <vector>
#include "Mesh.h"
#include "Material.h"
#include <memory>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>

class Model
{
public:

	Model(const char* objFilePath, glm::vec3 position);

	std::vector<std::shared_ptr<Mesh>> getMeshes();

private:
	std::vector<std::shared_ptr<Mesh>> meshes;
	std::vector<std::shared_ptr<Material>> materials;

	glm::vec3 position;

	void processNode(aiNode* node, const aiScene* scene);
	void loadModel(const char* objFilePath);

	std::shared_ptr<Mesh> createMesh(aiMesh* mesh, const aiScene* scene);


};

