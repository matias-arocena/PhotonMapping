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
	Mesh(float* vertexBuffer, unsigned* indexBuffer, const RTCGeometry& geometry, std::shared_ptr<Material> material);
	Mesh();
	virtual ~Mesh();

	RTCGeometry getGeometry();
	void setGeometryId(unsigned id);
	unsigned getGeometryId();

	std::shared_ptr<Material> getMaterial();

	// void LoadGeometry()
	
protected:
	RTCGeometry geometry;
	float* vertexBuffer;
	unsigned* indexBuffer;
	unsigned geometryId;
	std::shared_ptr<Material> material;

};

