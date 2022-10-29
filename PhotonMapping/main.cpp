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
#include <thread>
#include <chrono>
#include <thread>


int main()
{
	importScene();
	std::srand(time(0));
	omp_set_nested(1);

	auto t1 = std::thread([] {
		std::cout << "Generating Caustic" << std::endl;
		Scene::getInstance().generateCausticPhotonMap();
		std::cout << "Middle Caustic" << std::endl;

		std::vector<glm::vec3> buffer0 = Scene::getInstance().getCausticPhotonMap()->getMapBuffer();
		Scene::getInstance().saveImage(buffer0, "causticphotonmapping.png");
		std::cout << "Finish Caustic" << std::endl;
		});
	
	auto t2 = std::thread([] {
		std::cout << "Generating Global" << std::endl;
		Scene::getInstance().generateGlobalPhotonMap();
		std::cout << "Middle Global" << std::endl;

		std::vector<glm::vec3> buffer1 = Scene::getInstance().getGlobalPhotonMap()->getMapBuffer();
		Scene::getInstance().saveImage(buffer1, "globalphotonmapping.png");
		std::cout << "Finish Global" << std::endl;

		});
	
	t1.join();
	t2.join();

	std::vector<glm::vec3> buffer2 = Scene::getInstance().renderScene();
	Scene::getInstance().saveImage(buffer2, "raytracing.png");
	
}