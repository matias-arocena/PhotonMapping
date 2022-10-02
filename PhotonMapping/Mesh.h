#pragma once
#include <embree3/rtcore.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <memory>

class Mesh
{
public:
	Mesh(aiMesh* mesh, const aiScene* scene, RTCDevice device);

	~Mesh();

	RTCGeometry GetGeometry();

	// void LoadGeometry()


private:
	RTCGeometry Geometry;
	float* VertexBuffer;
	unsigned* IndexBuffer;
};
