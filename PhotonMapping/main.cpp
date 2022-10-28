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
#include <omp.h>


int main()
{
	importScene();
	std::srand(time(0));
	
	Scene::getInstance().generateCausticPhotonMap();
	std::vector<glm::vec3> buffer0 = Scene::getInstance().getCausticPhotonMap()->getMapBuffer();
	Scene::getInstance().saveImage(buffer0, "causticphotonmapping.png");

	Scene::getInstance().generateGlobalPhotonMap();
	std::vector<glm::vec3> buffer1 = Scene::getInstance().getGlobalPhotonMap()->getMapBuffer();
	Scene::getInstance().saveImage(buffer1, "globalphotonmapping.png");
	
	std::vector<glm::vec3> buffer2 = Scene::getInstance().renderScene();
	Scene::getInstance().saveImage(buffer2, "raytracing.png");
	
}