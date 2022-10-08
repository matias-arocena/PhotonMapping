#include "Loader.h"
#include <iostream>
#include <glm/glm.hpp>
#include "Settings.h"
#include "PointLight.h"
#include "TriangleLight.h"
#include "Camera.h"
#include "SquareLight.h"


void importModels(pugi::xml_node node)
{
	//Models
	for (pugi::xml_node obj : node.child("Models"))
	{
		std::string objType = obj.name();

		if (objType.compare("Model") == 0)
		{
			glm::vec3 position(0, 0, 0);
			position.x = obj.attribute("posx").as_float();
			position.y = obj.attribute("posy").as_float();
			position.z = obj.attribute("posz").as_float();

			float reflection = obj.attribute("reflection").as_float();
			float refraction = obj.attribute("refraction").as_float();

			std::string objRoute = obj.attribute("objRoute").as_string();

			Scene::getInstance().addModel(objRoute, position, reflection, refraction);
		}
	}
}


void importLights(pugi::xml_node node)
{
	//Lights
	for (pugi::xml_node obj : node.child("Lights"))
	{
		std::string objType = obj.name();
		
		glm::vec3 color;
		
		color.r = obj.attribute("colorr").as_float();
		color.g = obj.attribute("colorg").as_float();
		color.b = obj.attribute("colorb").as_float();

		float intensity = obj.attribute("intensity").as_float();
		int maximumEmittedPhotons = obj.attribute("maximumEmittedPhotons").as_int();

		if (objType.compare("PointLight") == 0)
		{

			glm::vec3 position;
			position.x = obj.attribute("x").as_float();
			position.y = obj.attribute("y").as_float();
			position.z = obj.attribute("z").as_float();

			float intensity = obj.attribute("intensity").as_float();
			int maximumEmittedPhotons = obj.attribute("maximumEmittedPhotons").as_int();

			std::shared_ptr<PointLight> pointLight = std::make_shared<PointLight>(position, intensity, maximumEmittedPhotons, color);

			Scene::getInstance().addLight(pointLight);
		}

		if (objType.compare("TriangleLight") == 0)
		{
			glm::vec3 p0{ 0,0,0 }, p1{ 0,0,0 }, p2{ 0,0,0 }, direction{ 0,0,0 }, color{0, 0, 0};
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

			std::shared_ptr<TriangleLight> triangleLight = std::make_shared<TriangleLight>(p0, p1, p2, direction, intensity, maximumEmittedPhotons, color);

			Scene::getInstance().addLight(triangleLight);
		}

		if (objType.compare("SquareLight") == 0)
		{
			float intensity = obj.attribute("intensity").as_float();

			glm::vec3 center = {
				obj.attribute("cenx").as_float(),
				obj.attribute("ceny").as_float(),
				obj.attribute("cenz").as_float(),
			};
			glm::vec3 normal = {
				obj.attribute("norx").as_float(),
				obj.attribute("nory").as_float(),
				obj.attribute("norz").as_float(),
			}; 
			glm::vec3 v = {
				obj.attribute("vx").as_float(),
				obj.attribute("vy").as_float(),
				obj.attribute("vz").as_float(),
			};

			std::shared_ptr<SquareLight> squareLight = std::make_shared<SquareLight>(intensity, maximumEmittedPhotons, color, center, normal, v);

			Scene::getInstance().addLight(squareLight);
		}
	}
}


void importCamera(pugi::xml_node node)
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

	Scene::getInstance().setCamera(camera);
}

void importScene()
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
	Settings::smoothness = doc.child("Scene").attribute("smoothness").as_int();

	importModels(doc.child("Scene"));
	importLights(doc.child("Scene"));
	importCamera(doc.child("Scene"));

	std::cout << "Scene loaded" << std::endl;
}