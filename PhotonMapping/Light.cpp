#include "Light.h"


Light::Light(glm::vec3 position, float intensity)
{
	this->position = position;
	this->intensity = intensity;

}

glm::vec3 Light::getPosition()
{
	return position;
}

float Light::getIntensity()
{
	return intensity;
}
