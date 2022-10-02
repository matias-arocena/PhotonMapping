#pragma once
#include <glm/glm.hpp>
#include <utility>

class Camera
{

private:
	glm::vec3 position;
	glm::vec3 direction;
	glm::vec3 upVector;

public:
	Camera(glm::vec3 position, glm::vec3 direction, glm::vec3 upVector);
	glm::vec3 getPosition();
	glm::vec3 getDirection();
	struct SecreenBase {
		glm::vec3 origin;
		glm::vec3 upVector;
		glm::vec3 rightVector;
	};
	SecreenBase screenBaseVectors();
};