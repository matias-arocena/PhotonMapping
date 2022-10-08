#include "Light.h"
#include "Scene.h"

Light::Light(float intensity, int maximumEmittedPhotons, glm::vec3 color)
{
	this->maximumEmittedPhotons = maximumEmittedPhotons;
	this->intensity = intensity;
	this->color = color;
}

float Light::getIntensity()
{
	return intensity;
}

void Light::emitPhotons()
{
	int emittedPhotons = 0;
	while (emittedPhotons <= maximumEmittedPhotons)
	{
		glm::vec3 direction = getPhotonDirection();
		glm::vec3 position = getPosition();
		Photon::trace(position, direction, color * (intensity / maximumEmittedPhotons));
		emittedPhotons += 1;
	}
}

Light::~Light(){}

glm::vec3 Light::getPhotonDirection() const
{
	return glm::vec3();
}

glm::vec3 Light::getPosition() const
{
	return glm::vec3();
}
