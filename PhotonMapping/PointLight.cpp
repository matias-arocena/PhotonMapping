#include "PointLight.h"
#include <random>
#define _USE_MATH_DEFINES
#include <math.h>

glm::vec3 PointLight::getPhotonDirection() const
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float>dist(-1.f, 1.f);

	glm::vec3 direction;
	do {
		direction.x = dist(gen);
		direction.y = dist(gen);
		direction.z = dist(gen);
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
