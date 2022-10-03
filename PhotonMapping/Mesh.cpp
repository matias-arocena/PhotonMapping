#include "Mesh.h"

#ifdef _DEBUG
#include <iostream>
#endif

Mesh::Mesh(aiMesh* mesh, const aiScene* scene, RTCDevice device, glm::vec3 position)
{
#ifdef _DEBUG
	std::cout << "Created Mesh" << std::endl;
#endif

	Geometry = rtcNewGeometry(device, RTC_GEOMETRY_TYPE_TRIANGLE);

	VertexBuffer = (float*)rtcSetNewGeometryBuffer(Geometry, RTC_BUFFER_TYPE_VERTEX, 0, RTC_FORMAT_FLOAT3, 3 * sizeof(float), mesh->mNumVertices);
	IndexBuffer = (unsigned*)rtcSetNewGeometryBuffer(Geometry, RTC_BUFFER_TYPE_INDEX, 0, RTC_FORMAT_UINT3, 3 * sizeof(unsigned), mesh->mNumFaces);

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		this->VertexBuffer[i * 3] = mesh->mVertices[i].x + position.x;
		this->VertexBuffer[i * 3 + 1] = mesh->mVertices[i].y + position.y;
		this->VertexBuffer[i * 3 + 2] = mesh->mVertices[i].z + position.z;
	}

	// Asumo que mesh siempre tiene triángulos
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		this->IndexBuffer[i * 3] = face.mIndices[0];
		this->IndexBuffer[i * 3 + 1] = face.mIndices[1];
		this->IndexBuffer[i * 3 + 2] = face.mIndices[2];
	}

	rtcCommitGeometry(Geometry);

}

Mesh::~Mesh()
{
	rtcReleaseGeometry(Geometry);

#if _DEBUG
	std::cout << "Delete Mesh" << std::endl;
#endif
}

RTCGeometry Mesh::GetGeometry()
{
	return Geometry;
}


