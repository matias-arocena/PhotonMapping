#include "Ray.h"
#include <embree3/rtcore.h>

Ray::Ray(glm::vec3 org, glm::vec3 dir) : origin{org}, direction{dir}
{
	rayHit = new RTCRayHit();
	rayHit->ray.org_x = org.x; 
	rayHit->ray.org_y = org.y; 
	rayHit->ray.org_z = org.z;
	rayHit->ray.dir_x = dir.x; 
	rayHit->ray.dir_y = dir.y; 
	rayHit->ray.dir_z = dir.z;
	rayHit->ray.tnear = 0.0f;
	rayHit->ray.tfar = std::numeric_limits<float>::infinity();
	rayHit->hit.geomID = RTC_INVALID_GEOMETRY_ID;
}

Ray::~Ray()
{
	delete rayHit;
}

bool Ray::getHit(glm::vec3 &OutHitCoordinates) const
{
	if (rayHit->hit.geomID != RTC_INVALID_GEOMETRY_ID) {
		OutHitCoordinates = origin + rayHit->ray.tfar * direction;
		return true;
	}
	else
	{
		return false;
	}
}

bool Ray::getHit() const
{
	return rayHit->hit.geomID != RTC_INVALID_GEOMETRY_ID;
}

RTCRayHit* Ray::getRayHit() const
{
	return rayHit;
}

glm::vec3 Ray::getNormal() const 
{
	if (rayHit->hit.geomID != RTC_INVALID_GEOMETRY_ID)
	{
		return glm::normalize(glm::vec3(
			rayHit->hit.Ng_x,
			rayHit->hit.Ng_y,
			rayHit->hit.Ng_z
		));
	}
	else
	{
		return glm::vec3(0,0,0);
	}
}