#include "Camera.h"
#define _USE_MATH_DEFINES
#include "Settings.h"
#include <math.h>

Camera::Camera(glm::vec3 position, glm::vec3 direction, glm::vec3 upVector)
{
	this->position = position;
	this->direction = glm::normalize(direction);
	this->upVector = glm::normalize(upVector);
}

glm::vec3 Camera::getPosition()
{
	return this->position;
}

glm::vec3 Camera::getDirection()
{
	return this->direction;
}

Camera::SecreenBase Camera::screenBaseVectors()
{
	float dist = 1;
	glm::vec3 screenOrigin = { 0,0,0 };
	screenOrigin = position + dist * direction;
	glm::vec3 rightVector = -glm::cross(direction, upVector);
	rightVector = glm::normalize(rightVector);
	return { screenOrigin, upVector, rightVector };
}

std::vector<std::shared_ptr<Ray>> Camera::generateRaysCamera()
{
	std::vector<std::shared_ptr<Ray>> rays;
	Camera::SecreenBase screenBase = screenBaseVectors();
	float aspectRatio = ((float)Settings::height / Settings::width);
	float scale = 2 * tanf(static_cast<float>(Settings::fov * M_PI / 360.0f) * 0.5f);
	for (int j = 0; j < Settings::height; j++)
	{
		for (int i = 0; i < Settings::width; i++)
		{
			glm::vec3 pixelPos;
			float x = ((i / (float)Settings::width) - 0.5f) * scale;
			float y = ((j / (float)Settings::height) - 0.5f) * scale * aspectRatio;
			pixelPos = screenBase.origin + screenBase.rightVector * x + screenBase.upVector * y;
			auto ray = std::make_shared<Ray>(this->position, glm::normalize(pixelPos - this->position));
			rays.push_back(ray);
		}
	}
	return rays;
}


