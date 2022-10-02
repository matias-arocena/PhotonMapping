#include <embree3/rtcore.h>
#include <limits>
#include <iostream>
#include <FreeImage.h>

#include "Camera.h"
#include "Scene.h"
#include "Model.h"
#include "Ray.h"
#include "Settings.h"
#include "Loader.h"
#include <assimp/importer.hpp>




int main()
{

	RTCDevice device = rtcNewDevice(nullptr);

	std::shared_ptr<Scene> scene = std::make_shared<Scene>(device);

	importScene(scene);

	const char* objFilePath = "untitled.obj";
	std::shared_ptr<Model> Triangle = std::make_shared<Model>(objFilePath, device);

	scene->AttachModel(std::move(Triangle));

	scene->Commit();

	Ray FirstRay(glm::vec3{ 0.0f, 0.0f, -3.0f }, glm::vec3{ 0.0f, 0.0f, 1.f });

	Camera camera(glm::vec3(0.0f, -5.0f, 0.0f), glm::vec3(0.0f, 1.0f, .0f), glm::vec3( 0.0f, .0f, 1.0f));

	std::vector<Ray> camRays = camera.generateRaysCamera();
	std::vector<glm::vec3> buffer;

	for (Ray camRay : camRays)
	{
		scene->ThrowRay(camRay);

		glm::vec3 HitCoordinates;

		if (camRay.GetHit(HitCoordinates))
		{
			buffer.push_back(glm::vec3{ 255,255,255 });
			//std::cout << HitCoordinates.x << "," << HitCoordinates.y << "," << HitCoordinates.z << std::endl;
		}
		else
		{
			buffer.push_back(glm::vec3{ 0,0,0 });
			//std::cout << "No hit" << std::endl;
		}
	}

	scene->saveImage(buffer);

	rtcReleaseDevice(device);
}
