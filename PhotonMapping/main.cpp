#include <embree3/rtcore.h>
#include <limits>
#include <iostream>

#include "Scene.h"
#include "Model.h"
#include <assimp/importer.hpp>
int main()
{
	const int image_width = 1024, image_height = 12;

	RTCDevice device = rtcNewDevice(nullptr);

	Scene Scene(device);

	const char* objFilePath = "untitled.obj";
	std::shared_ptr<Model> Triangle = std::make_shared<Model>(objFilePath, device);

	Scene.AttachModel(std::move(Triangle));

	Scene.Commit();

	RTCRayHit rayhit;
	rayhit.ray.org_x = 0.f; rayhit.ray.org_y = 0.f; rayhit.ray.org_z = -3.f;
	rayhit.ray.dir_x = 0.f; rayhit.ray.dir_y = 0.f; rayhit.ray.dir_z = 1.f;
	rayhit.ray.tnear = 0.f;
	rayhit.ray.tfar = std::numeric_limits<float>::infinity();
	rayhit.hit.geomID = RTC_INVALID_GEOMETRY_ID;


	
	RTCIntersectContext context;
	rtcInitIntersectContext(&context);

	rtcIntersect1(Scene.GetScene(), &context, &rayhit);

	if (rayhit.hit.geomID != RTC_INVALID_GEOMETRY_ID) {
		
		std::cout << "Intersection at t = " << rayhit.ray.tfar << std::endl;
	}
	else
	{
		std::cout << "No intersection." << std::endl;
	}

	rtcReleaseDevice(device);
}
