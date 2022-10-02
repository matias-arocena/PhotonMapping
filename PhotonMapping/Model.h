#pragma once
#include <vector>
#include "Mesh.h"
#include <memory>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>

class Model
{
public:

	Model(const char* objFilePath, RTCDevice device);

	std::vector<std::shared_ptr<Mesh>> getMeshes();

private:
	std::vector<std::shared_ptr<Mesh>> meshes;

	void processNode(aiNode* node, const aiScene* scene, RTCDevice device);

	void loadModel(const char* objFilePath, RTCDevice device);

};

