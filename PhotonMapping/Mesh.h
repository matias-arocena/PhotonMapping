#pragma once
#include <embree3/rtcore.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Material.h"
#include <glm/glm.hpp>
#include <memory>

class Mesh
{
public:
	Mesh(aiMesh* mesh, const aiScene* scene, RTCDevice device, glm::vec3 position);
	Mesh(float* vertexBuffer, unsigned* indexBuffer, const RTCGeometry& geometry, std::shared_ptr<Material> material);

	~Mesh();

	RTCGeometry GetGeometry();
	void setGeometryId(unsigned id);
	unsigned getGeometryId();

	std::shared_ptr<Material> getMaterial();

	// void LoadGeometry()


private:
	RTCGeometry geometry;
	float* vertexBuffer;
	unsigned* indexBuffer;
	unsigned geometryId;
	std::shared_ptr<Material> material;
};

