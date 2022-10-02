#include "PointLight.h"




PointLight::PointLight(glm::vec3 position, float intensity):Light(intensity)
{
	this->position = position;
}

PointLight::~PointLight()
{
}

glm::vec3 PointLight::getPosition()
{
	return position;
}