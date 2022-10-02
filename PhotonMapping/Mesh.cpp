#include "Mesh.h"

#ifdef _DEBUG
#include <iostream>
#endif

Mesh::Mesh(aiMesh* mesh, const aiScene* scene, RTCDevice device)
{
#ifdef _DEBUG
	std::cout << "Created Mesh" << std::endl;
#endif

	Geometry = rtcNewGeometry(device, RTC_GEOMETRY_TYPE_TRIANGLE);

	VertexBuffer = (float*)rtcSetNewGeometryBuffer(Geometry, RTC_BUFFER_TYPE_VERTEX, 0, RTC_FORMAT_FLOAT3, 3 * sizeof(float), 3);
	IndexBuffer = (unsigned*)rtcSetNewGeometryBuffer(Geometry, RTC_BUFFER_TYPE_INDEX, 0, RTC_FORMAT_UINT3, 3 * sizeof(unsigned), 1);

	unsigned int vrtx_qty = mesh->mVertices->Length();
	this->VertexBuffer = new float[vrtx_qty * 3];
	for (unsigned int i = 0; i < vrtx_qty; i++)
	{
		this->VertexBuffer[i * 3] = mesh->mVertices[i].x;
		this->VertexBuffer[i * 3 + 1] = mesh->mVertices[i].y;
		this->VertexBuffer[i * 3 + 2] = mesh->mVertices[i].z;
	}

	// Asumo que mesh siempre tiene triángulos
	unsigned int indx_qty = mesh->mNumFaces;
	this->IndexBuffer = new unsigned int[indx_qty * 3];
	for (unsigned int i = 0; i < indx_qty; i++)
	{
		aiFace face = mesh->mFaces[i];

		this->VertexBuffer[i * 3] = face.mIndices[i];
		this->VertexBuffer[i * 3 + 1] = (unsigned int)face.mIndices[i];
		this->VertexBuffer[i * 3 + 2] = (unsigned int)face.mIndices[i];
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


