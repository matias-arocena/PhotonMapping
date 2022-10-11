#include "PointLight.h"
#include "Random.h"
#define _USE_MATH_DEFINES
#include <math.h>

glm::vec3 PointLight::getPhotonDirection() const
{
	glm::vec3 direction;
	do {
		direction.x = Random::getValueFromMinus1To1();
		direction.y = Random::getValueFromMinus1To1();
		direction.z = Random::getValueFromMinus1To1();
	} while (direction.x * direction.x + direction.y * direction.y + direction.z * direction.z > 1);

	return direction;
}

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
