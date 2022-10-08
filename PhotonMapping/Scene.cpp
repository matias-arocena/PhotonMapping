#include "Scene.h"

#include <FreeImage.h>
#include "Model.h"
#include "Ray.h"
#include "Settings.h"
#include "Device.h"
#include <iostream>
#endif
#include "SquareLight.h"

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

void Scene::saveImage(std::vector<glm::vec3> buffer)
{
    FreeImage_Initialise();
    FIBITMAP* bitmap = FreeImage_Allocate(Settings::width, Settings::height, 24);
    RGBQUAD color;
    for (int j = 0; j < Settings::height; j++)
    {
        for (int i = 0; i < Settings::width; ++i)
        {
            color.rgbRed = static_cast<BYTE>((buffer)[(j * Settings::width) + i].r);
            color.rgbBlue = static_cast<BYTE>((buffer)[(j * Settings::width) + i].b);
            color.rgbGreen = static_cast<BYTE>((buffer)[(j * Settings::width) + i].g);
            FreeImage_SetPixelColor(bitmap, i, j, &color);
        }
    }

    if (FreeImage_Save(FIF_PNG, bitmap, "imagen_prueba1.png", 0))
    {
        std::cout << "Image " << "imagen_prueba1.png" << " saved" << std::endl;
    }
    FreeImage_DeInitialise();
}


RTCScene Scene::getScene()
{
	return scene;
}

void Scene::attachModel(std::shared_ptr<Model> model)
{
	std::vector<std::shared_ptr<Mesh>> meshes = model->getMeshes();
	for ( std::shared_ptr<Mesh> mesh : meshes)
	{
		int id = rtcAttachGeometry(scene, mesh->GetGeometry());
        mesh->setGeometryId(id);
        std::cout << "id: " << id << std::endl;
	}
	
}

void Scene::commit()
{
	rtcCommitScene(scene);
}

void Scene::throwRay(Ray& Ray)
{
	rtcIntersect1(scene, &context, Ray.getRayHit());
}

void Scene::addLight(std::shared_ptr<Light> light)
{
	std::shared_ptr<SquareLight> squareLight = std::dynamic_pointer_cast<SquareLight>(light);
	if (squareLight)
	{
		squareLight->createEmbreeMesh(device);
		int id = rtcAttachGeometry(TheScene, squareLight->getGeometry());
		RTCError error = rtcGetDeviceError(device);
		squareLight->setGeometryId(id);
	}
    Lights.push_back(light);
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
    for (auto model : this->models)
    {
        for (auto mesh : model->getMeshes())
        {
            if (mesh->getGeometryId() == id)
            {
                return mesh;
            }
        }
    }
    return NULL;
}

bool Scene::getMeshWithGeometryID(unsigned id, Mesh &mesh)
{
	for (auto model : this->Models)
	{
		for (auto msh : model->getMeshes())
		{
			if (msh->getGeometryId() == id)
			{
				mesh = *msh;
				return true;
			}
		}
	}
	return false;
}



glm::vec3 colorToRgb(const glm::vec3& color)
{
    return glm::clamp(glm::vec3(color.r * 255, color.g * 255, color.b * 255), glm::vec3(0, 0, 0), glm::vec3(255, 255, 255));
}





std::vector<glm::vec3> Scene::renderScene()
{
    std::vector<glm::vec3> buffer;

    std::vector<Ray> camRays = camera->generateRaysCamera();
    for (Ray camRay : camRays)
    {

        buffer.push_back(colorToRgb(trace(camRay, Settings::maxDepth, 1.0f)));

    }
    return buffer;
}


float computePointLightIntensity(glm::vec3 L, const glm::vec3& normal, const float& pointLigntInt)
{
	double lightInt; // Light intensity
	double fatt = 1 / pow(L.length(), 2); // Factor distancia entre luz y punto
	L = glm::normalize(L);
	lightInt = glm::max(glm::dot(normal, L), 0.f) * fatt * pointLigntInt; // pointLight->getIntensity(); // El dot product de 2 vectores normalizados da el coseno del angulo entre ellos, 50 es la intensidad

	lightInt = glm::clamp(lightInt, 0.0, 1.0);
	return static_cast<float>(lightInt);
}

float computeSpecularPointLightIntensity(
	const glm::vec3& L, const glm::vec3& normal, 
	const Ray& r,
	const float& lightInt, const float& specularExponent, const float& specularFactor
){
	glm::vec3 R = glm::reflect(L, normal);
	float specAmmount = glm::dot(r.direction, R);
	if (specAmmount < 0) specAmmount = 0;
	return pow(specAmmount, specularExponent) * specularFactor * lightInt;
}


glm::vec3 Scene::trace(Ray ray, int depth, float currentRefract)
{
	glm::vec3 color = Settings::backgroundColor; 
	if (depth <= 0)
	{
		return color;
	}

	throwRay(ray);

	glm::vec3 HitCoordinates;

	if (ray.getHit(HitCoordinates))
	{
		RTCRayHit* hit = ray.getRayHit();
		auto mesh = getMeshWithGeometryID(hit->hit.geomID);
		auto material = mesh->getMaterial();

		color += shade(ray, material, depth, currentRefract);
	}

	return color;
	
}


glm::vec3 Scene::shade(const Ray& r, std::shared_ptr<Material> material, int depth, float currentRefract) {
	// Ambient light
	glm::vec3 color = glm::vec3(0,0,0);
	glm::vec3 normal = r.getNormal();
	glm::vec3 hitPos;
	r.getHit(hitPos);


	for (auto light : lights)
	{
		/*
		Point light
		*/
		std::shared_ptr<PointLight> pointLight = std::dynamic_pointer_cast<PointLight>(light);
		if (pointLight != NULL)
		{
			glm::vec3 L = pointLight->getPosition() - hitPos;
			
			Ray shadowRay = Ray(hitPos + (normal * 0.01f), glm::normalize(L)); // Ray from hit to light source

			glm::vec3 shadowHitPos;
			throwRay(shadowRay);
			
			Mesh *mesh = NULL;
			if (shadowRay.getHit(shadowHitPos) && glm::distance(hitPos, shadowHitPos) <= glm::distance(pointLight->getPosition(), shadowHitPos) && getMeshWithGeometryID(shadowRay.GetRayHit()->hit.geomID, *mesh))
			{
				float opacity = 1;
				RTCRayHit* hit = shadowRay.getRayHit();
				auto hitMaterial = mesh->getMaterial();
				opacity = hitMaterial->getOpacity();

				auto obst_dist = glm::distance(hitPos, shadowHitPos);
				auto light_dist = glm::distance(pointLight->getPosition(), shadowHitPos);

				if (opacity < 1 || glm::distance(hitPos, shadowHitPos) <= glm::distance(pointLight->getPosition(), shadowHitPos))
				{
					// Diffuse Light intensity
					float lightInt = computePointLightIntensity(L, normal, light->getIntensity()) * (1 - opacity);

					// Phong Specular intensity
					L = glm::normalize(L);
					float specInt = computeSpecularPointLightIntensity(L, normal, r, lightInt, hitMaterial->getSpecularExponent(), hitMaterial->getSpecularFactor());

					color += (material->getDiffuse() * lightInt) + specInt;
				}
			}
			else
			{
				// Diffuse Light intensity
				float lightInt = computePointLightIntensity(L, normal, light->getIntensity());

				L = glm::normalize(L);
				// Phong Specular intensity
				float specInt = computeSpecularPointLightIntensity(L, normal, r, lightInt, material->getSpecularExponent(), material->getSpecularFactor());

				color += (material->getDiffuse() * lightInt) + specInt;

			}

		}
		/* Square light */
		std::shared_ptr<SquareLight> squareLight = std::dynamic_pointer_cast<SquareLight>(light);
		if (squareLight != NULL)
		{



		}

	}

	if (material->getOpacity() < 1) {

		glm::vec3 reflect = glm::reflect(r.direction, normal);
		glm::vec3 refract = glm::refract(r.direction, normal, currentRefract / material->getRefraction());

		Ray reflectRay(hitPos + (r.Direction * -0.01f), reflect);
		Ray refractRay(hitPos + (r.Direction * -0.01f), refract);

		color += trace(reflectRay, depth - 1, material->getRefraction());
		color += trace(refractRay, depth - 1, material->getRefraction());
	}
	else if (material->getReflection())
	{
		glm::vec3 reflect = glm::reflect(r.Direction, normal);
		Ray reflectRay(hitPos + (r.Direction * -0.01f), reflect);
		glm::vec3 reflect = glm::reflect(r.direction, normal);
		Ray reflectRay(hitPos + (normal * 0.01f), reflect);
		color += trace(reflectRay, depth - 1, currentRefract) * material->getReflection();
	}

	/*
	double D_N = dot(r.d, p->normal);
	if (profundidad > 0) {
		
			Se computa la reflectividad del objeto
		
		if (p->material->Kr > 0) {
			vec3 dRefl = r.d - (p->normal * 2 * D_N);
			Ray refl(p->point, dRefl);
			refl.o += refl.d * 0.0001;
			color += traza(refl, profundidad - 1, COLOR_R) * p->material->Kr;
		}

		
			Se computa la refracción del objeto
		
		if (p->material->Kt > 0) {
			double cosi = D_N;
			if (D_N > 1) D_N = 1;
			if (D_N < -1) D_N = -1;
			double etai = 1, etat = p->material->Ir;
			vec3 n = p->normal;
			if (cosi < 0) { cosi = -cosi; }
			else { std::swap(etai, etat); n = vec3(0, 0, 0) - p->normal; }
			double eta = etai / etat;
			double k = 1 - eta * eta * (1 - cosi * cosi);

			if (k >= 0) {
				vec3 refDir = ((r.d * eta) + (n * (eta * cosi - sqrtf(k))));
				Ray refraccion(p->point, refDir);
				refraccion.o += refDir * 0.001;
				color += traza(refraccion, profundidad - 1, COLOR_R) * p->material->Kt;
			}
		}
	}



	if (color.getX() > 1) color.x = 1;
	if (color.getY() > 1) color.y = 1;
	if (color.getZ() > 1) color.z = 1;
	delete p;

	*/
	return glm::clamp(color, glm::vec3(0, 0, 0), glm::vec3(1, 1, 1));
}

