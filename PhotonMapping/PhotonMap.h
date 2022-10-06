#pragma once

#include "glm/glm.hpp"
#include "KDTree.h"

#include <string>
#include <iostream>

struct Photon
{
	glm::vec3 position;
	char power[4];
	char phi, theta;

	/*
	phi = 256 * atan2(dy,dx) / (2*PI) 
	theta = 256 * acos(dz) / PI
	*/

	friend std::ostream& operator<<(std::ostream& o, const Photon& a);
	void LoadFromString(std::string photonString);
	
private:
	static std::vector<std::string> split(const std::string& s, char delim);
};


