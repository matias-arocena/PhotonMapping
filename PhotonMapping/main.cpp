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
#include "KdTree.h"
#include "PhotonMap.h"
#include <assimp/importer.hpp>



int main()
{
	importScene();
	Scene::getInstance().photonMapping();
	std::cout << "Photon Mapping" << std::endl;
	std::vector<glm::vec3> buffer = Scene::getInstance().renderScene();
	std::cout << "RayTracing" << std::endl;
	Scene::getInstance().saveImage(buffer, "raytracing.png");


}