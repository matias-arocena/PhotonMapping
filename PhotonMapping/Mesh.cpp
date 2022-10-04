#include "Mesh.h"

#ifdef _DEBUG
#include <iostream>
#endif

Mesh::Mesh(float* vertexBuffer, unsigned* indexBuffer, const RTCGeometry& geometry, std::shared_ptr<Material> material)
{
	this->vertexBuffer = vertexBuffer;
	this->indexBuffer = indexBuffer;
	this->geometry = geometry;
	this->material = material;
}

Mesh::~Mesh()
{
	rtcReleaseGeometry(geometry);

#if _DEBUG
	std::cout << "Delete Mesh" << std::endl;
#endif
}

void Mesh::setGeometryId(unsigned id)
{
	geometryId = id;
}

unsigned Mesh::getGeometryId()
{
	return geometryId;
}

RTCGeometry Mesh::GetGeometry()
{
	return geometry;
}

std::shared_ptr<Material> Mesh::getMaterial()
{
	return this->material;
}


