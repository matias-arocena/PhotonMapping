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

glm::vec3 Camera::getUp() 
{
	return this->upVector;
}

void Camera::setPosition(glm::vec3 position)
{
	this->position = position;
}

void Camera::setDirection(glm::vec3 direction)
{
	this->direction = direction;
}

void Camera::setUp(glm::vec3 upVector)
{
	this->upVector = upVector;
}

Camera::SecreenBase Camera::screenBaseVectors()
{
	float dist = 1;
	glm::vec3 screenOrigin = { 0,0,0 };
	screenOrigin = position + dist * direction;
	glm::vec3 rightVector = glm::cross(direction, upVector);
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

std::vector<std::vector<std::shared_ptr<Ray>>> Camera::generateRaysCameraWithAA()
{
	std::vector<std::vector<std::shared_ptr<Ray>>> rays;
	Camera::SecreenBase screenBase = screenBaseVectors();
	float aspectRatio = ((float)Settings::height / Settings::width);
	float scale = 2 * tanf(static_cast<float>(Settings::fov * M_PI / 360.0f) * 0.5f);
	for (int j = 0; j < Settings::height; j++)
	{
		for (int i = 0; i < Settings::width; i++)
		{
			float newI, newJ;
			float x, y;
			glm::vec3 pixelPos;
			std::shared_ptr<Ray> r;
			std::vector<std::shared_ptr<Ray>> ray;
		
			newI = static_cast<float>(i);
			newJ = static_cast<float>(j);
			x = ((newI / (float)Settings::width) - 0.5f) * scale;
			y = ((newJ / (float)Settings::height) - 0.5f) * scale * aspectRatio;
			pixelPos = screenBase.origin + screenBase.rightVector * x + screenBase.upVector * y;
			r = std::make_shared<Ray>(this->position, glm::normalize(pixelPos - this->position));
			ray.push_back(r);

			newI = i + 0.5f;
			x = ((newI / (float)Settings::width) - 0.5f) * scale;
			y = ((newJ / (float)Settings::height) - 0.5f) * scale * aspectRatio;
			pixelPos = screenBase.origin + screenBase.rightVector * x + screenBase.upVector * y;
			r = std::make_shared<Ray>(this->position, glm::normalize(pixelPos - this->position));
			ray.push_back(r);

			newJ = j + 0.5f;
			x = ((newI / (float)Settings::width) - 0.5f) * scale;
			y = ((newJ / (float)Settings::height) - 0.5f) * scale * aspectRatio;
			pixelPos = screenBase.origin + screenBase.rightVector * x + screenBase.upVector * y;
			r = std::make_shared<Ray>(this->position, glm::normalize(pixelPos - this->position));
			ray.push_back(r);

			newI = i + 0.5f;
			newJ = j + 0.5f;
			x = ((newI / (float)Settings::width) - 0.5f) * scale;
			y = ((newJ / (float)Settings::height) - 0.5f) * scale * aspectRatio;
			pixelPos = screenBase.origin + screenBase.rightVector * x + screenBase.upVector * y;
			r = std::make_shared<Ray>(this->position, glm::normalize(pixelPos - this->position));
			ray.push_back(r);


			rays.push_back(ray);
		}
	}
	return rays;
}


