#include "Loader.h"
#include <iostream>
#include <glm/glm.hpp>
#include "Settings.h"
#include "PointLight.h"
#include "TriangleLight.h"
#include "Camera.h"


void importModels(pugi::xml_node node, std::shared_ptr<Scene> scene)
{
	//Models
	for (pugi::xml_node obj : node.child("Models"))
	{
		std::string objType = obj.name();

		if (objType.compare("Model"))
		{
			float xPos = obj.attribute("x").as_float();
			float yPos = obj.attribute("y").as_float();
			float zPos = obj.attribute("z").as_float();

			std::string objRoute = obj.attribute("objRoute").as_string();

			//pasarle el path a model y agregarlo a la scena
		}
	}
}


void importLights(pugi::xml_node node, std::shared_ptr<Scene> scene)
{
	//Lights
	for (pugi::xml_node obj : node.child("Lights"))
	{
		std::string objType = obj.name();

		if (objType.compare("PointLight") == 0)
		{
			glm::vec3 position;
			position.x = obj.attribute("x").as_float();
			position.y = obj.attribute("y").as_float();
			position.z = obj.attribute("z").as_float();

			float intensity = obj.attribute("intensity").as_float();

			std::shared_ptr<PointLight> pointLight = std::make_shared<PointLight>(position, intensity);

			scene->addLight(pointLight);
		}

		if (objType.compare("TriangleLight") == 0)
		{
			glm::vec3 p0{ 0,0,0 }, p1{ 0,0,0 }, p2{ 0,0,0 }, direction{ 0,0,0 };
			direction.x = obj.attribute("dirx").as_float();
			direction.y = obj.attribute("diry").as_float();
			direction.z = obj.attribute("dirz").as_float();

			p0.x = obj.attribute("p0x").as_float();
			p0.y = obj.attribute("p0y").as_float();
			p0.z = obj.attribute("p0z").as_float();

			p1.x = obj.attribute("p1x").as_float();
			p1.y = obj.attribute("p1y").as_float();
			p1.z = obj.attribute("p1z").as_float();

			p2.x = obj.attribute("p2x").as_float();
			p2.y = obj.attribute("p2y").as_float();
			p2.z = obj.attribute("p2z").as_float();

			float intensity = obj.attribute("intensity").as_float();

			std::shared_ptr<TriangleLight> triangleLight = std::make_shared<TriangleLight>(p0, p1, p2, direction, intensity);

			scene->addLight(triangleLight);
		}
	}
}


void importCamera(pugi::xml_node node, std::shared_ptr<Scene> scene)
{
	// camera
	pugi::xml_node obj = node.child("Camera");
	std::string objType = obj.name();

	glm::vec3 position{ 0,0,0 }, direction{ 0,0,0 }, up{ 0,0,0 };

	position.x = obj.attribute("x").as_float();
	position.y = obj.attribute("y").as_float();
	position.z = obj.attribute("z").as_float();
	direction.x = obj.attribute("dirx").as_float();
	direction.y = obj.attribute("diry").as_float();
	direction.z = obj.attribute("dirz").as_float();
	up.x = obj.attribute("upx").as_float();
	up.y = obj.attribute("upy").as_float();
	up.z = obj.attribute("upz").as_float();

	std::shared_ptr<Camera> camera = std::make_shared<Camera>(position, direction, up);

	scene->setCamera(camera);
}

void importScene(std::shared_ptr<Scene> scene)
{
	pugi::xml_document doc;

	pugi::xml_parse_result result = doc.load_file("Assets/config.xml");

	if (!result)
	{
		std::cout << "Failed to load scene xml" << std::endl;
		std::cout << "Error description: " << result.description() << "\n";
		return;
	}

	//General Settings
	Settings::width = doc.child("Scene").attribute("resx").as_int();
	Settings::height = doc.child("Scene").attribute("resy").as_int();
	Settings::maxDepth = doc.child("Scene").attribute("maxDepth").as_int();
	Settings::backgroundColor = {
		doc.child("Scene").attribute("bgColorR").as_int(),
		doc.child("Scene").attribute("bgColorG").as_int(),
		doc.child("Scene").attribute("bgColorB").as_int(),
	};

	Settings::fov = doc.child("Scene").attribute("fov").as_int();

	importModels(doc.child("Scene"), scene);
	importLights(doc.child("Scene"), scene);
	importCamera(doc.child("Scene"), scene);

	std::cout << "Scene loaded" << std::endl;
}