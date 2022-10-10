#pragma once
#include <glm/glm.hpp>
#include <FreeImage.h>
#include <chrono>

class Settings
{
public:
	static int width;
	static int height;
	static int maxDepth;
	static glm::vec3 backgroundColor;
	static int fov;
	static int antialiasing;
	static int smoothness;
	static int photonQuantity;
};