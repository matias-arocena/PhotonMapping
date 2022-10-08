#include "Light.h"
#include <embree3/rtcore_geometry.h>

#pragma once
class SquareLight : public Light
{
public:
	SquareLight(float intensity, glm::vec3 center, glm::vec3 normal, glm::vec3 v);

	// Returns a random position of the light, could be used to throw photons or for ray tracing shading
	glm::vec3* getRandomPositions(unsigned qty);
	glm::vec3 getNormal();
	void setGeometryId(unsigned id);
	RTCGeometry getGeometry();


	void createEmbreeMesh(RTCDevice device);

private:
	glm::vec3 center;
	glm::vec3 normal;
	glm::vec3 v, u;

	RTCGeometry geometry;
	float* vertexBuffer;
	unsigned* indexBuffer;
	unsigned geometryId;

};

