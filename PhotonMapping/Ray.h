#pragma once
#include <glm/glm.hpp>
#include <embree3/rtcore.h>

class Ray {
public:
	Ray(glm::vec3 org, glm::vec3 dir);
	~Ray();
	bool GetHit(glm::vec3& OutHitCoordinates) const; 
	bool GetHit() const; 
	RTCRayHit* GetRayHit() const;
	glm::vec3 Origin;
	glm::vec3 Direction;

	glm::vec3 getNormal() const;

private:
	RTCRayHit* RayHit;

};











