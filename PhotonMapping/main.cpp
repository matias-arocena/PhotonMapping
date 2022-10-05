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

	std::vector<glm::vec3> buffer = scene->renderScene();
	scene->saveImage(buffer);

	rtcReleaseDevice(scene->device);
}
