#pragma once
#include "Light.h"

class TringleLight : public Light
{
	glm::vec3 direction;
	glm::vec3 p0;
	glm::vec3 p1;
	glm::vec3 p2;

public:
	TringleLight(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 direction, float intensity);
	~TringleLight();
	
};

