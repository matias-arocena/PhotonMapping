#pragma once

#include <glm/glm.hpp>
#include "Light.h"


class PointLight : public Light
{

	glm::vec3 position;

public:

	PointLight(glm::vec3 position, float intensity, int maximumEmittedPhotons, glm::vec3 color);
	~PointLight();

	glm::vec3 getPosition() const;

};

