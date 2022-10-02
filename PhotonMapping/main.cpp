#include <embree3/rtcore.h>
#include <limits>
#include <iostream>
#include <FreeImage.h>

#include "Camera.h"
#include "Scene.h"
#include "Model.h"
#include "Ray.h"
#include "Settings.h"
#include <assimp/importer.hpp>



int main()
{

	RTCDevice device = rtcNewDevice(nullptr);

	Scene Scene(device);

	const char* objFilePath = "untitled.obj";
	std::shared_ptr<Model> Triangle = std::make_shared<Model>(objFilePath, device);

	Scene.AttachModel(std::move(Triangle));

	Scene.Commit();

	Ray FirstRay(glm::vec3{ 0.0f, 0.0f, -3.0f }, glm::vec3{ 0.0f, 0.0f, 1.f });

	Camera camera(glm::vec3(0.0f, 0.0f, -3.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3( 0.0f, 1.0f, 0.0f));

	std::vector<Ray> camRays = camera.generateRaysCamera();
	std::vector<glm::vec3> buffer;

	for (Ray camRay : camRays)
	{
		Scene.ThrowRay(camRay);

		glm::vec3 HitCoordinates;

		if (camRay.GetHit(HitCoordinates))
		{
			buffer.push_back(glm::vec3{ 255,255,255 });
			std::cout << HitCoordinates.x << "," << HitCoordinates.y << "," << HitCoordinates.z << std::endl;
		}
		else
		{
			buffer.push_back(glm::vec3{ 0,0,0 });
			//std::cout << "No hit" << std::endl;
		}
	}

	rtcReleaseDevice(device);
}
