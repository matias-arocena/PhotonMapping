#pragma once
#include <glm/glm.hpp>


class Light
{

private:
	glm::vec3 position;
	float intensity;
	

public:
	Light(glm::vec3 position, float intensity);
	glm::vec3 getPosition();
	float getIntensity();
};
