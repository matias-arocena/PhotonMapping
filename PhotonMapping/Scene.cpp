#include "Scene.h"

#include <FreeImage.h>
#include "Model.h"
#include "Ray.h"
#include "Settings.h"
#include "Device.h"
#include "SquareLight.h"
#include <iostream>
#include <glm/gtc/constants.hpp>
#include <omp.h>


Scene::Scene()
{
	scene = rtcNewScene(Device::getInstance().getDevice());
	rtcInitIntersectContext(&context);
}

Scene& Scene::getInstance()
{
	static Scene instance;
	return instance;
}

Scene::~Scene()
{
	rtcReleaseScene(scene);
}

void Scene::saveImage(std::vector<glm::vec3> buffer, std::string path)
{
    FreeImage_Initialise();
    FIBITMAP* bitmap = FreeImage_Allocate(Settings::width, Settings::height, 24);
    
	for (int j = 0; j < Settings::height; j++)
    {
        for (int i = 0; i < Settings::width; ++i)
        {
			RGBQUAD color;
            color.rgbRed = static_cast<BYTE>((buffer)[(j * Settings::width) + i].r);
            color.rgbBlue = static_cast<BYTE>((buffer)[(j * Settings::width) + i].b);
            color.rgbGreen = static_cast<BYTE>((buffer)[(j * Settings::width) + i].g);
            FreeImage_SetPixelColor(bitmap, i, j, &color);
        }
    }

    if (FreeImage_Save(FIF_PNG, bitmap, path.c_str(), 0))
    {
#ifdef  _DEBUG

        std::cout << "Image " << path << " saved" << std::endl;
#endif // _DEBUG

	}
    FreeImage_DeInitialise();
}


RTCScene Scene::getScene()
{
	return scene;
}

void Scene::attachModel(std::shared_ptr<Model> model)
{
	std::vector<std::shared_ptr<Mesh>> modMeshes = model->getMeshes();
	for ( std::shared_ptr<Mesh> mesh : modMeshes)
	{
		int id = meshes.size();
		meshes.push_back(mesh);
		rtcAttachGeometryByID(scene, mesh->getGeometry(), id);
        mesh->setGeometryId(id);
#ifdef _DEBUG
        std::cout << "Geometry id: " << id << std::endl;
#endif // _DEBUG

	}
	
}

void Scene::commit()
{
	rtcCommitScene(scene);
}

void Scene::throwRay(std::shared_ptr<Ray> ray)
{
	rtcIntersect1(scene, &context, ray->getRayHit());
}

void Scene::addLight(std::shared_ptr<Light> light)
{
	std::shared_ptr<SquareLight> squareLight = std::dynamic_pointer_cast<SquareLight>(light);
	if (squareLight)
	{
		int id = meshes.size();
		meshes.push_back(squareLight);
		RTCDevice device = Device::getInstance().getDevice();
		squareLight->createEmbreeMesh(device);
		rtcAttachGeometryByID(scene, squareLight->getGeometry(), id);
		squareLight->setGeometryId(id);
#ifdef _DEBUG
		std::cout << "Light Geometry id: " << id << std::endl;
#endif // DEBUG

	}
    lights.push_back(light);
	commit();
}

void Scene::setCamera(std::shared_ptr<Camera> camera)
{
    this->camera = camera;
}

std::shared_ptr<Camera> Scene::getCamera()
{
	return camera;
}


void Scene::addModel(std::string objRoute, glm::vec3 position, float reflection, float refraction)
{
    std::shared_ptr<Model> model = std::make_shared<Model>(objRoute.c_str(), position);

    models.push_back(model);

    attachModel(std::move(model));

    commit();
}

std::shared_ptr<Mesh> Scene::getMeshWithGeometryID(unsigned id)
{
	if (id < meshes.size())
	{
		return meshes[id];
	} 
	else
	{
		return nullptr;
	}
}


glm::vec3 colorToRgb(const glm::vec3& color)
{
    return glm::clamp(glm::vec3(color.r * 255, color.g * 255, color.b * 255), glm::vec3(0, 0, 0), glm::vec3(255, 255, 255));
}


std::shared_ptr<PhotonMap> Scene::getGlobalPhotonMap()
{
	return global;
}

std::shared_ptr<PhotonMap> Scene::getCausticPhotonMap()
{
	return caustic;
}

void Scene::generateGlobalPhotonMap()
{
	std::shared_ptr<PhotonMap> photonMap = std::make_shared<PhotonMap>();
	float globalIntensity = 0;

	for (auto light : lights)
	{
		globalIntensity += light->getIntensity();
	}

	for (int i = 0; i < lights.size(); ++i)
	{
		lights[i]->setPhotonQuantity((lights[i]->getIntensity() / globalIntensity) * Settings::globalPhotonQuantity);
		lights[i]->emitPhotons(photonMap, false);
	}

	photonMap->build();
	photonMap->absCount = 0;

	global = photonMap;
}

void Scene::setGlobalPhotonMap(std::shared_ptr<PhotonMap> photonMap)
{
	photonMap->build();
	global = photonMap;
}

void Scene::generateCausticPhotonMap()
{
	std::shared_ptr<PhotonMap> photonMap = std::make_shared<PhotonMap>();
	float globalIntensity = 0;

	for (auto light : lights)
	{
		globalIntensity += light->getIntensity();
	}

	for (int i = 0; i < lights.size(); ++i)
	{
		lights[i]->setPhotonQuantity((lights[i]->getIntensity() / globalIntensity) * Settings::causticPhotonQuantity);
		lights[i]->emitPhotons(photonMap, true);
	}

	photonMap->build();
	photonMap->absCount = 0;

	caustic = photonMap;
}

void Scene::setCausticPhotonMap(std::shared_ptr<PhotonMap> photonMap)
{
	photonMap->build();
	caustic = photonMap;
}

std::vector<glm::vec3> Scene::renderScene()
{
	std::vector<glm::vec3> buffer(Settings::width * Settings::height, glm::vec3{ 0,0,0 });
	std::vector<std::vector<std::shared_ptr<Ray>>> camRays = camera->generateRaysCameraWithAA();


#pragma omp parallel for
	for (int i = 0; i < camRays.size(); ++i)
	{
		glm::vec3 color = Settings::backgroundColor;
		for (int j = 0; j < 4; j++)
		{
			color += trace(camRays[i][j], Settings::maxDepth, 1.0f);
		}
		buffer[i] = colorToRgb(color / 4.f);

	}

    return buffer;
}


float computePointLightIntensity(glm::vec3 L, const glm::vec3& normal, const float& pointLigntInt)
{
	double lightInt; // Light intensity

	double fatt = glm::length(L) * glm::length(L); // Factor distancia entre luz y punto 1 / d^2
	
	L = glm::normalize(L);
	lightInt = glm::max(glm::dot(normal, L), 0.f) * pointLigntInt / fatt; // pointLight->getIntensity(); // El dot product de 2 vectores normalizados da el coseno del angulo entre ellos, 50 es la intensidad

	lightInt = glm::clamp(lightInt, 0.0, 1.0);
	return static_cast<float>(lightInt);
}

float computeSquareLightIntensity(glm::vec3 L, const glm::vec3& normal, const float& pointLigntInt, std::shared_ptr<SquareLight> light) {
	double lightInt; // Light intensity

	double fatt = glm::length(L) * glm::length(L); // Factor distancia entre luz y punto 1 / d^2

	L = glm::normalize(L);
	float dot = glm::dot(-L, light->getNormal());

	lightInt = glm::max(glm::dot(normal, L), 0.f) * dot * pointLigntInt / fatt; // pointLight->getIntensity(); // El dot product de 2 vectores normalizados da el coseno del angulo entre ellos, 50 es la intensidad

	lightInt = glm::clamp(lightInt, 0.0, 1.0);
	return static_cast<float>(lightInt);

}

float computeSpecularPointLightIntensity(
	const glm::vec3& L, const glm::vec3& normal, 
	std::shared_ptr<Ray> r,
	const float& lightInt, const float& specularExponent, const float& specularFactor
){
	glm::vec3 R = glm::reflect(L, normal);
	float specAmmount = glm::dot(r->direction, R);
	if (specAmmount < 0) specAmmount = 0;
	return pow(specAmmount, specularExponent) * specularFactor * lightInt;
}


glm::vec3 Scene::trace(std::shared_ptr<Ray> ray, const int &depth, const float &currentRefract)
{
	glm::vec3 color = Settings::backgroundColor; 
	if (depth <= 0)
	{
		return color;
	}

	throwRay(ray);

	glm::vec3 HitCoordinates;

	if (ray->getHit(HitCoordinates))
	{
		RTCRayHit* hit = ray->getRayHit();
		
		std::shared_ptr<Mesh> mesh = NULL;
		std::shared_ptr<SquareLight> squareLight = NULL;

		if (hit->hit.geomID < meshes.size())
		{
			mesh = meshes[hit->hit.geomID];
			squareLight = std::dynamic_pointer_cast<SquareLight>(mesh);
		}

		if (squareLight == NULL)
		{
			auto material = mesh->getMaterial();

			color = shade(ray, material, depth, currentRefract);
		}
		else
		{
			color = squareLight->getColor();
		}
	}

	return color;
	
}


glm::vec3 Scene::computeShadow(const glm::vec3 &lightPosition, const glm::vec3 &normal, const glm::vec3 &hitPos, const float &intensity, std::shared_ptr<Material> material, std::shared_ptr<Ray> r, const int &lightId, glm::vec3 lightColor)
{
	glm::vec3 color = Settings::backgroundColor;
	glm::vec3 L = lightPosition - hitPos;

	auto shadowRay = std::make_shared<Ray>(hitPos + (normal * 0.01f), glm::normalize(L)); // Ray from hit to light source

	glm::vec3 shadowHitPos;
	throwRay(shadowRay);

	std::shared_ptr<Mesh> mesh = NULL;
	std::shared_ptr<SquareLight> squareLight = NULL;
	if (shadowRay->getHit(shadowHitPos) && shadowRay->getRayHit()->hit.geomID < meshes.size())
	{
		mesh = meshes[shadowRay->getRayHit()->hit.geomID];
		squareLight = std::dynamic_pointer_cast<SquareLight>(mesh);
	}

	if (shadowRay->getHit(shadowHitPos) && 
		glm::distance(hitPos, shadowHitPos) <= glm::distance(lightPosition, shadowHitPos) && 
		squareLight == NULL && 
		shadowRay->getRayHit()->hit.geomID != lightId)
	{
		float opacity = 1;
		RTCRayHit* hit = shadowRay->getRayHit();
		auto hitMaterial = mesh->getMaterial();
		opacity = hitMaterial->getOpacity();

		auto obst_dist = glm::distance(hitPos, shadowHitPos);
		auto light_dist = glm::distance(lightPosition, shadowHitPos);

		if (opacity < 1 || glm::distance(hitPos, shadowHitPos) <= glm::distance(lightPosition, shadowHitPos))
		{
			// Diffuse Light intensity
			float lightInt = computePointLightIntensity(L, normal, intensity) * (1 - opacity);

			// Phong Specular intensity
			L = glm::normalize(L);
			float specInt = computeSpecularPointLightIntensity(L, normal, r, lightInt, hitMaterial->getSpecularExponent(), hitMaterial->getSpecularFactor());

			color = lightColor * ((material->getDiffuse() * lightInt) + specInt);
		}
	}
	else if (squareLight == NULL || shadowRay->getRayHit()->hit.geomID == lightId)
	{
		float lightInt = 0;
		// Diffuse Light intensity
		if (squareLight != NULL)
		{
			lightInt = computeSquareLightIntensity(L, normal, intensity, squareLight);
		}
		else
		{
			lightInt = computePointLightIntensity(L, normal, intensity);
		}

		L = glm::normalize(L);
		// Phong Specular intensity
		float specInt = computeSpecularPointLightIntensity(L, normal, r, lightInt, material->getSpecularExponent(), material->getSpecularFactor());

		color = lightColor * ((material->getDiffuse() * lightInt) + specInt);

	}
	return color;
}



glm::vec3 Scene::shade(std::shared_ptr<Ray> r, std::shared_ptr<Material> material, int depth, float currentRefract) {
	// Ambient light
	glm::vec3 color = Settings::backgroundColor;
	glm::vec3 normal = r->getNormal();
	glm::vec3 hitPos;
	r->getHit(hitPos);

	if (material->getOpacity() == 1) {
		for (int i = 0; i < lights.size(); ++i)
		{
			/*
			Point light
			*/
			std::shared_ptr<PointLight> pointLight = std::dynamic_pointer_cast<PointLight>(lights[i]);
			if (pointLight != NULL)
			{
				color += computeShadow(pointLight->getPosition(), normal, hitPos, pointLight->getIntensity(), material, r, -1, pointLight->getColor());
			}

			/* Square light */
			std::shared_ptr<SquareLight> squareLight = std::dynamic_pointer_cast<SquareLight>(lights[i]);
			if (squareLight != NULL)
			{


				std::vector<glm::vec3> corners = squareLight->getCorners();
				bool finishShadow = false;
				float intensity = squareLight->getIntensity() / corners.size();
				glm::vec3 tempColor = Settings::backgroundColor;
				for (int i = 0; i < corners.size() && finishShadow; i++)
				{
					glm::vec3 point = corners[i];
					glm::vec3 cornerColor = computeShadow(point, normal, hitPos, intensity, material, r, squareLight->getGeometryId(), squareLight->getColor());
					if (cornerColor == Settings::backgroundColor)
					{
						finishShadow = false;
					}
					tempColor += cornerColor;
				}

				if (!finishShadow)
				{
					std::vector<glm::vec3> points = squareLight->getRandomPositions(Settings::smoothness);

					float intensity = squareLight->getIntensity() / points.size();
					for (int j = 0; j < points.size(); ++j)
					{
						color += computeShadow(points[j], normal, hitPos, intensity, material, r, squareLight->getGeometryId(), squareLight->getColor());
					}
				}
				else
				{
					color += tempColor;
				}
			}

		}
	}

	if (material->getOpacity() < 1) {

		//glm::vec3 reflect = glm::reflect(r->direction, normal);

		// La normal se tiene que invertir cuando se esta adentro del objeto refractado
		// Esta solucion esta mal, funciona solo cuando el ior del material vidrio es distinto de 1
		// Y tampoco toma en cuenta cuando dos objetos transparentes se chocan
		glm::vec3 refract = { 0,0,0 };
		if (currentRefract != 1)
		{
			refract = glm::refract(r->direction, -normal, currentRefract / material->getRefraction());
		}
		else
		{
			refract = glm::refract(r->direction, normal, currentRefract / material->getRefraction());
		}
		
		//auto reflectRay = std::make_shared<Ray>(hitPos + (r->direction * -0.01f), reflect);
		auto refractRay = std::make_shared<Ray>(hitPos + refract * 0.01f, refract);

		//color += trace(reflectRay, depth - 1, material->getReflection());
		color += trace(refractRay, depth - 1, material->getRefraction());
	}
	else if (material->getReflection())
	{
		glm::vec3 reflect = glm::reflect(r->direction, normal);
		auto reflectRay = std::make_shared<Ray>(hitPos + (r->direction * -0.01f), reflect);
		color += trace(reflectRay, depth - 1, currentRefract) * material->getReflection();
	}

	if (material->getOpacity() == 1) {
		// Photon map
		float r2;
		std::vector<int> photonIndices = global->queryKNearestPhotons(hitPos, 1000, r2);
		glm::vec3 totalPower = Settings::backgroundColor;
		if (photonIndices.size() > 0)
		{
			for (int photonIndex : photonIndices)
			{
				auto photon = global->getPhoton(photonIndex);
				//float fatt = 10 * pow(glm::length(photon->position - hitPos), 2) / r2; // TODO: Ta bien esto?
				totalPower += photon->power * glm::abs(glm::dot(normal, photon->incidentDirection)); // Incident direction affects photon power contribution
			}
		}

		float area = (glm::pi<float>() * r2);

		color += totalPower / area;

		// Caustic Photon map
		photonIndices = caustic->queryKNearestPhotons(hitPos, 10, r2);
		totalPower = Settings::backgroundColor;
		if (photonIndices.size() > 0)
		{
			for (int photonIndex : photonIndices)
			{
				auto photon = caustic->getPhoton(photonIndex);
				//float fatt = 10 * pow(glm::length(photon->position - hitPos), 2) / r2; // TODO: Ta bien esto?
				totalPower += photon->power * glm::abs(glm::dot(normal, photon->incidentDirection)); // Incident direction affects photon power contribution
			}
		}

		area = (glm::pi<float>() * r2);

		color += totalPower / area;
	}

	return glm::clamp(color, glm::vec3(0, 0, 0), glm::vec3(1, 1, 1));
}

