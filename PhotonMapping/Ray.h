#pragma once
#include <glm/glm.hpp>
#include <embree3/rtcore.h>

class Ray {
public:
	Ray(glm::vec3 org, glm::vec3 dir);
	~Ray();
	bool getHit(glm::vec3& OutHitCoordinates) const; 
	bool getHit() const; 
	RTCRayHit* getRayHit() const;
	glm::vec3 origin;
	glm::vec3 direction;

	glm::vec3 getNormal() const;

private:
	RTCRayHit* rayHit;

};











