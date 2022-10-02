#pragma once
#include <glm/glm.hpp>


class Light
{

private:
	float intensity;
	

public:
	Light(float intensity);
	float getIntensity();
};
