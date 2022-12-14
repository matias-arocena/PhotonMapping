#pragma once
#include <glm/glm.hpp>
#include <utility>
#include <vector>
#include "Ray.h"

#include <memory>


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
	glm::vec3 getUp();
	void setPosition(glm::vec3 position);
	void setDirection(glm::vec3 direction);
	void setUp(glm::vec3 upVector);
	struct SecreenBase {
		glm::vec3 origin;
		glm::vec3 upVector;
		glm::vec3 rightVector;
	};
	SecreenBase screenBaseVectors();

	std::vector<std::shared_ptr<Ray>> generateRaysCamera();
	std::vector<std::vector<std::shared_ptr<Ray>>> generateRaysCameraWithAA();
};