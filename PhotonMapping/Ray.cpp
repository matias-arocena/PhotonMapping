#include "Ray.h"
#include <embree3/rtcore.h>

Ray::Ray(glm::vec3 org, glm::vec3 dir) : Origin{org}, Direction{dir}
{
	RayHit = new RTCRayHit();
	RayHit->ray.org_x = org.x; 
	RayHit->ray.org_y = org.y; 
	RayHit->ray.org_z = org.z;
	RayHit->ray.dir_x = dir.x; 
	RayHit->ray.dir_y = dir.y; 
	RayHit->ray.dir_z = dir.z;
	RayHit->ray.tnear = 0.0f;
	RayHit->ray.tfar = std::numeric_limits<float>::infinity();
	RayHit->hit.geomID = RTC_INVALID_GEOMETRY_ID;
}

Ray::~Ray()
{
	delete RayHit;
}

bool Ray::GetHit(glm::vec3 &OutHitCoordinates)
{
	if (RayHit->hit.geomID != RTC_INVALID_GEOMETRY_ID) {
		OutHitCoordinates = Origin + RayHit->ray.tfar * Direction;
		return true;
	}
	else
	{
		return false;
	}
}

RTCRayHit* Ray::GetRayHit()
{
	return RayHit;
}

