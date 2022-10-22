#pragma once

#include "glm/glm.hpp"
#include "KDTree.h"

#include <string>
#include <iostream>
#include <memory>
#include <random>
#include "Scene.h"

class Scene;
class PhotonMap;

enum ReflectionType {
	Diffuse,
	Specular,
	Absortion
};

struct Photon
{
	glm::vec3 position;
	glm::vec3 power;
	glm::vec3 incidentDirection;

	friend std::ostream& operator<<(std::ostream& o, const Photon& a);
	void loadFromString(std::string photonString);
	static void trace(glm::vec3 lightPosition, glm::vec3 lightPhotonDirection, glm::vec3 power, int depth, float currentRefract, std::shared_ptr<PhotonMap> photonMap);

private:
	static std::vector<std::string> split(const std::string& s, char delim);

	ReflectionType russianRoulete(std::shared_ptr<Material> material, float value);
};

class PhotonMap {
private:
	std::vector<std::shared_ptr<Photon>> photons;
	KdTree kdtree;

public:
	unsigned absCount = 0;
	void addPhoton(std::shared_ptr<Photon> photon);
	void build();

	const int getSize() const;
	const std::shared_ptr<Photon> getPhoton(int i) const;

	std::vector<int> queryKNearestPhotons(const glm::vec3& p, int k, float& maxDist2);
	std::vector<int> queryKNearestPhotons(const glm::vec3& p, float radius, float& maxDist2);

	std::vector<glm::vec3> getMapBuffer();
};
