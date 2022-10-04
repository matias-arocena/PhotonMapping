#pragma once

#include "glm/glm.hpp"
#include "KDTree.h"

struct Photon
{
	glm::vec3 position;
	char power[4];
	char phi, theta;

	/*
	phi = 256 * atan2(dy,dx) / (2*PI) 
	theta = 256 * acos(dz) / PI
	*/
};

