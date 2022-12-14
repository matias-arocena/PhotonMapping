#include "Light.h"
#include "Mesh.h"
#include <vector>
#include <embree3/rtcore_geometry.h>

#pragma once
class SquareLight : public Light , public Mesh
{
public:
	SquareLight(float intensity, unsigned maximumEmittedPhotons, glm::vec3 color, glm::vec3 center, glm::vec3 normal, glm::vec3 v);

	// Returns a random position of the light, could be used to throw photons or for ray tracing shading
	std::vector<glm::vec3> getRandomPositions(int qty);
	glm::vec3 getNormal();


	void createEmbreeMesh(RTCDevice device);
	std::vector<glm::vec3> getCorners();

private:
	virtual glm::vec3 getPosition() const override;
	virtual glm::vec3 getPhotonDirection() const override;

	glm::vec3 center;
	glm::vec3 normal;
	glm::vec3 v, u;

};

