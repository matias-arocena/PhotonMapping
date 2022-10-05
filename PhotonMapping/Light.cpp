#include "Light.h"


Light::Light(float intensity)
{
	this->intensity = intensity;
}

float Light::getIntensity()
{
	return intensity;
}

Light::~Light(){}