#pragma once

#include <glm/glm.hpp>
#include "Light.h"


class PointLight : public Light
{

	glm::vec3 position;

	virtual glm::vec3 getPhotonDirection() const override;


public:

	PointLight(glm::vec3 position, float intensity, int maximumEmittedPhotons, glm::vec3 color);
	~PointLight();

	virtual glm::vec3 getPosition() const override;

};

