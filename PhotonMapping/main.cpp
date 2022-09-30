#include <embree3/rtcore.h>
#include <limits>
#include <iostream>

#include "Model.h"

int main()
{
	RTCDevice device = rtcNewDevice(nullptr);
	RTCScene scene = rtcNewScene(device);

	std::shared_ptr<Model> Triangle = std::make_shared<Model>(device);

	rtcAttachGeometry(scene, Triangle->GetGeometry());
	Triangle.reset();

	rtcCommitScene(scene);

	RTCRayHit rayhit;
	rayhit.ray.org_x = 0.f; rayhit.ray.org_y = 0.f; rayhit.ray.org_z = -3.f;
	rayhit.ray.dir_x = 0.f; rayhit.ray.dir_y = 0.f; rayhit.ray.dir_z = 1.f;
	rayhit.ray.tnear = 0.f;
	rayhit.ray.tfar = std::numeric_limits<float>::infinity();
	rayhit.hit.geomID = RTC_INVALID_GEOMETRY_ID;

	RTCIntersectContext context;
	rtcInitIntersectContext(&context);

	rtcIntersect1(scene, &context, &rayhit);

	if (rayhit.hit.geomID != RTC_INVALID_GEOMETRY_ID) {
		std::cout << "Intersection at t = " << rayhit.ray.tfar << std::endl;
	}
	else
	{
		std::cout << "No intersection." << std::endl;
	}

	rtcReleaseScene(scene);
	rtcReleaseDevice(device);
}
