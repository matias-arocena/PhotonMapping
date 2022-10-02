#pragma once
#include "Light.h"

class TriangleLight : public Light
{
	glm::vec3 direction;
	glm::vec3 p0;
	glm::vec3 p1;
	glm::vec3 p2;

public:
	TriangleLight(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 direction, float intensity);
	~TriangleLight();
	
};

