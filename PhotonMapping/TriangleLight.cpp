#include "TriangleLight.h"

TriangleLight::TriangleLight(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 direction, float intensity, int maximumEmittedPhotons, glm::vec3 color):
	Light(intensity, maximumEmittedPhotons, color)
{
	this->direction = direction;
	this->p0 = p0;
	this->p1 = p1;
	this->p2 = p2;
}

TriangleLight::~TriangleLight()
{
}
