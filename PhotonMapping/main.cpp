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


void generatePhotonMapping()
{

	auto t1 = std::thread([] {
		Scene::getInstance().generateCausticPhotonMap();

		std::vector<glm::vec3> buffer0 = Scene::getInstance().getCausticPhotonMap()->getMapBuffer();
		Scene::getInstance().saveImage(buffer0, "causticphotonmapping.png");
		});

	auto t2 = std::thread([] {
		Scene::getInstance().generateGlobalPhotonMap();

		std::vector<glm::vec3> buffer1 = Scene::getInstance().getGlobalPhotonMap()->getMapBuffer();
		Scene::getInstance().saveImage(buffer1, "globalphotonmapping.png");

		});

	t1.join();
	t2.join();
}

void generateRayTracing()
{
	std::vector<glm::vec3> buffer2 = Scene::getInstance().renderScene();
	Scene::getInstance().saveImage(buffer2, "raytracing.png");
}
void importPhotonMap()
{

	auto t1 = std::thread([] {
		std::shared_ptr<PhotonMap> global = std::make_shared<PhotonMap>();
		global->importKdTree("global.txt");
		Scene::getInstance().setGlobalPhotonMap(global);
		});

	auto t2 = std::thread([] {
		std::shared_ptr<PhotonMap> caustic = std::make_shared<PhotonMap>();
		caustic->importKdTree("global.txt");
		Scene::getInstance().setCausticPhotonMap(caustic);
		});

	t1.join();
	t2.join();
}

void exportPhotonMap()
{

	auto t1 = std::thread([] {
		Scene::getInstance().getGlobalPhotonMap()->exportKdTree("global.txt");
		});

	auto t2 = std::thread([] {
		Scene::getInstance().getCausticPhotonMap()->exportKdTree("caustic.txt");
		});
	t1.join();
	t2.join();
}

void modifyCamera()
{
	bool finish = false;
	while (!finish)
	{
		std::shared_ptr<Camera> camera = Scene::getInstance().getCamera();
		std::cout << "## Modificar Camara ##" << std::endl; 
		std::cout << "1 - Modificar Posicion ("
			<< camera->getPosition().x << ","
			<< camera->getPosition().y << ","
			<< camera->getPosition().z << "):" << std::endl;
		std::cout << "2 - Modificar Direcccion ("
			<< camera->getDirection().x << ","
			<< camera->getDirection().y << ","
			<< camera->getDirection().z << "):" << std::endl;
		std::cout << "3 - Modificar Up ("
			<< camera->getUp().x << ","
			<< camera->getUp().y << ","
			<< camera->getUp().z << "):" << std::endl;
		std::cout << "0 - Terminar"
			<< std::endl;
		int opt;
		std::cin >> opt;
		switch (opt) {
		case 0:
			finish = true;
			break;
		case 1:
			glm::vec3 newPos = camera->getPosition();
			std::cout << "Ingrese posicion x:";
			std::cin >> newPos.x;
			std::cout << "Ingrese posicion y:";
			std::cin >> newPos.y;
			std::cout << "Ingrese posicion z:";
			std::cin >> newPos.z;
			camera->setPosition(newPos);
			break;
		case 2:
			glm::vec3 newDir = camera->getPosition();
			std::cout << "Ingrese direccion x:";
			std::cin >> newDir.x;
			std::cout << "Ingrese direccion y:";
			std::cin >> newDir.y;
			std::cout << "Ingrese direccion z:";
			std::cin >> newDir.z;
			camera->setDirection(newDir);
			break;
		case 3:
			glm::vec3 newUp = camera->getPosition();
			std::cout << "Ingrese up x:";
			std::cin >> newUp.x;
			std::cout << "Ingrese up y:";
			std::cin >> newUp.y;
			std::cout << "Ingrese up z:";
			std::cin >> newUp.z;
			camera->setUp(newUp);
			break;
		default:
			std::cout << "Error: Opcion incorrecta." << std::endl;
			break;
		}

	}
}

int main()
{
	bool generated = false;
	bool isRunning = true;
	std::chrono::steady_clock::time_point begin, end;

	importScene();

	std::srand(static_cast<int>(time(0)));
	omp_set_nested(1);

	while (isRunning)
	{
		std::cout << "## Photon Mapping ## " << std::endl;
		std::cout << "1 - Generar Photon Mapping" << std::endl;
		std::cout << "2 - Generar RayTracing" << std::endl;
		std::cout << "3 - Generar Escena" << std::endl;
		std::cout << "4 - Importar Photon Map" << std::endl;
		std::cout << "5 - Exportar Photon Map" << std::endl;
		std::cout << "6 - Modificar Camara" << std::endl;
		std::cout << "0 - Salir" << std::endl;
		int opt;
		std::cin >> opt;
		switch (opt) {
		case 1:
			std::cout << "Generando Photon Map." << "\r" << std::flush;
			begin = std::chrono::steady_clock::now();
			generatePhotonMapping();
			end = std::chrono::steady_clock::now();
			std::cout << "Photon Map generado en " << std::chrono::duration_cast<std::chrono::milliseconds> (end - begin).count() << " ms" << std::endl;

			generated = true;
			break;
		case 2:
			if (!generated) {
				std::cout << "Error: El photon map no esta cargado." << std::endl;
			}
			else
			{
				std::cout << "Generando RayTracing." << std::endl;
				begin = std::chrono::steady_clock::now();

				generateRayTracing();

				end = std::chrono::steady_clock::now();
				std::cout << "RayTracing generado en " << std::chrono::duration_cast<std::chrono::milliseconds> (end - begin).count() << " ms" << std::endl;
			}
			break;
		case 3:
			std::cout << "Generando Photon Map." << std::endl;
			begin = std::chrono::steady_clock::now();

			generatePhotonMapping();

			end = std::chrono::steady_clock::now();
			std::cout << "Photon Map generado en " << std::chrono::duration_cast<std::chrono::milliseconds> (end - begin).count() << " ms" << std::endl;

			std::cout << "Generando RayTracing." << std::endl;
			begin = std::chrono::steady_clock::now();

			generateRayTracing();

			end = std::chrono::steady_clock::now();
			std::cout << "RayTracing generado en " << std::chrono::duration_cast<std::chrono::milliseconds> (end - begin).count() << " ms" << std::endl;

			break;
		case 4: 
			std::cout << "Importando Photon Map." << std::endl;
			importPhotonMap();

			std::cout << "Photon Map importado." << std::endl;
			generated = true;
			break;
		case 5:
			if (!generated)
			{
				generatePhotonMapping();
			}
			std::cout << "Exportando Photon Map.";

			exportPhotonMap();
			std::cout << "\rPhoton Map export.           " << std::endl;
			break;
		case 6:
			modifyCamera();
			break;
		case 0:
			isRunning = false;
			break;
		default:
			std::cout << "Error: Opcion incorrecta." << std::endl;
			break;
		}
	}

}