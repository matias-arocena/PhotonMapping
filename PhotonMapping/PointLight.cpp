#include "PointLight.h"


PointLight::PointLight(glm::vec3 position, float intensity, int maximumEmittedPhotons, glm::vec3 color):Light(intensity, maximumEmittedPhotons, color)
{
	this->position = position;
}

PointLight::~PointLight()
{
}

glm::vec3 PointLight::getPosition() const
{
	return position;
}