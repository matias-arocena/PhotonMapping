#include <embree3/rtcore.h>
#include <limits>
#include <iostream>

#include "Scene.h"
#include "Model.h"
#include "Ray.h"

int main()
{
	const int image_width = 1024, image_height = 12;

	RTCDevice device = rtcNewDevice(nullptr);

	Scene Scene(device);

	std::unique_ptr<Model> Triangle = std::make_unique<Model>(device);
	Scene.AttachModel(std::move(Triangle));

	Scene.Commit();

	Ray FirstRay(glm::vec3{ 0.0f, 0.0f, -3.0f }, glm::vec3{ 0.0f, 0.0f, 1.f });

	Scene.ThrowRay(FirstRay);

	glm::vec3 HitCoordinates;
	if (FirstRay.GetHit(HitCoordinates))
	{
		std::cout << HitCoordinates.x << "," << HitCoordinates.y << "," << HitCoordinates.z << std::endl;
	}
	else
	{
		std::cout << "No hit" << std::endl;
	}

	rtcReleaseDevice(device);
}
