#pragma once
#include <glm/glm.hpp>
#include <memory>

class PhotonMap;
class Light
{

private:
	float intensity;
	int maximumEmittedPhotons;
	glm::vec3 color;

public:
	Light(float intensity, int maximumEmittedPhotons, glm::vec3 color);
	void setPhotonQuantity(int quantity);
	float getIntensity();
	void emitPhotons(std::shared_ptr<PhotonMap> photonMap);
	virtual ~Light();
	glm::vec3 getColor();

protected:
	virtual glm::vec3 getPhotonDirection() const;
	virtual glm::vec3 getPosition() const;
};
