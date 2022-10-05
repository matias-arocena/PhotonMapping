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
}

bool Ray::GetHit(glm::vec3 &OutHitCoordinates) const
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

bool Ray::GetHit() const
{
	return RayHit->hit.geomID != RTC_INVALID_GEOMETRY_ID;
}

RTCRayHit* Ray::GetRayHit() const
{
	return RayHit;
}

glm::vec3 Ray::getNormal() const 
{
	if (RayHit->hit.geomID != RTC_INVALID_GEOMETRY_ID)
	{
		return glm::normalize(glm::vec3(
			RayHit->hit.Ng_x,
			RayHit->hit.Ng_y,
			RayHit->hit.Ng_z
		));
	}
	else
	{
		return glm::vec3(0,0,0);
	}
}