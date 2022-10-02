#include <glm/glm.hpp>
#include <embree3/rtcore.h>

class Ray {
public:
	Ray(glm::vec3 org, glm::vec3 dir);
	~Ray();
	bool GetHit(glm::vec3& OutHitCoordinates); 
	RTCRayHit* GetRayHit();

private:
	RTCRayHit* RayHit;

	glm::vec3 Origin;
	glm::vec3 Direction;
};











