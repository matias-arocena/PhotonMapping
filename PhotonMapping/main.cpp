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


glm::vec3 colorToRgb(const glm::vec3 &color)
{
	return glm::vec3(color.r * 255, color.g * 255, color.b * 255);
}


int main()
{
	RTCDevice device = rtcNewDevice(nullptr);

	std::shared_ptr<Scene> scene = std::make_shared<Scene>(device);

	importScene(scene);

	std::vector<Ray> camRays = scene->camera->generateRaysCamera();
	std::vector<glm::vec3> buffer;

	for (Ray camRay : camRays)
	{
		scene->ThrowRay(camRay);

		glm::vec3 HitCoordinates;

		if (camRay.GetHit(HitCoordinates))
		{
			RTCRayHit* hit = camRay.GetRayHit();
			auto mesh = scene->getMeshWithGeometryID(hit->hit.geomID);
			auto material = mesh->getMaterial();
			
			glm::vec3 color = material->getDiffuse();
			
			buffer.push_back(colorToRgb(color));
		}
		else
		{
			buffer.push_back(glm::vec3{ 0,0,0 });
			//std::cout << "No hit" << std::endl;
		}
	}

	scene->saveImage(buffer);

	rtcReleaseDevice(scene->device);
}
