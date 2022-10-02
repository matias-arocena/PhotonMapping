#pragma once

#include <glm/glm.hpp>
#include "Light.h"


class PointLight : public Light
{

	glm::vec3 position;

public:

	PointLight(glm::vec3 position, float intensity);
	~PointLight();

	glm::vec3 getPosition();

};

