#pragma once

#include "glm/glm.hpp"
#include "KDTree.h"

#include <string>
#include <iostream>
#include "Scene.h"

struct Photon
{
	glm::vec3 position;
	glm::vec3 power;
	char phi, theta;

	/*
	phi = 256 * atan2(dy,dx) / (2*PI) 
	theta = 256 * acos(dz) / PI
	*/

	friend std::ostream& operator<<(std::ostream& o, const Photon& a);
	void LoadFromString(std::string photonString);
	static void trace(glm::vec3 lightPosition, glm::vec3 lightPhotonDirection, glm::vec3 power);
private:
	static std::vector<std::string> split(const std::string& s, char delim);
};

class PhotonMap {
private:
	std::vector<Photon> photons;
	KdTree kdtree;

public:
	PhotonMap() {}

	void addPhoton(const Photon& p);
	void build();

	const int getSize() const;
	const Photon& getPhoton(int i) const;

	std::vector<int> queryKNearestPhotons(const glm::vec3& p, int k, float& maxDist2) const;

	std::vector<glm::vec3> getMapBuffer(Scene& scene);
};
