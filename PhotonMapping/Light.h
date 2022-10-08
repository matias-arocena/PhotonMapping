#pragma once
#include <glm/glm.hpp>


class Light
{

private:
	float intensity;
	int maximumEmittedPhotons;
	glm::vec3 color;

public:
	Light(float intensity, int maximumEmittedPhotons, glm::vec3 color);
	float getIntensity();
	void emitPhotons();
	virtual ~Light();

private:
	virtual glm::vec3 getPhotonDirection() const;
	virtual glm::vec3 getPosition() const;
};
