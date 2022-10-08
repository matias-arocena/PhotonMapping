#include "Scene.h"

#include <FreeImage.h>
#include "Model.h"
#include "Ray.h"
#include "Settings.h"
#include "Device.h"

#ifdef _DEBUG
#include <iostream>
#endif



Scene::Scene()
{
	TheScene = rtcNewScene(Device::getInstance().getDevice());
	rtcInitIntersectContext(&context);

#ifdef _DEBUG
	std::cout << "Created Scene" << std::endl;
#endif
}

Scene::~Scene()
{
	rtcReleaseScene(TheScene);

#ifdef _DEBUG
	std::cout << "Deleted Scene" << std::endl;
#endif

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
            color.rgbRed = (buffer)[(j * Settings::width) + i].r;
            color.rgbBlue = (buffer)[(j * Settings::width) + i].b;
            color.rgbGreen = (buffer)[(j * Settings::width) + i].g;
            FreeImage_SetPixelColor(bitmap, i, j, &color);
        }
    }

    if (FreeImage_Save(FIF_PNG, bitmap, "imagen_prueba1.png", 0))
    {
        std::cout << "Image " << "imagen_prueba1.png" << " saved" << std::endl;
    }
    FreeImage_DeInitialise();
}


RTCScene Scene::GetScene()
{
	return TheScene;
}

void Scene::AttachModel(std::shared_ptr<Model> model)
{
	std::vector<std::shared_ptr<Mesh>> meshes = model->getMeshes();
	for ( std::shared_ptr<Mesh> mesh : meshes)
	{
		int id = rtcAttachGeometry(TheScene, mesh->GetGeometry());
        mesh->setGeometryId(id);
        std::cout << "id: " << id << std::endl;
	}
	
}

void Scene::Commit()
{
	rtcCommitScene(TheScene);
}

void Scene::ThrowRay(Ray& Ray)
{
	rtcIntersect1(TheScene, &context, Ray.GetRayHit());
}

void Scene::addLight(std::shared_ptr<Light> light)
{
    Lights.push_back(light);
}

void Scene::setCamera(std::shared_ptr<Camera> camera)
{
    this->camera = camera;
}

std::shared_ptr<Camera> Scene::getCamera()
{
}


void Scene::addModel(std::string objRoute, glm::vec3 position, float reflection, float refraction)
{
    std::shared_ptr<Model> model = std::make_shared<Model>(objRoute.c_str(), position);

    Models.push_back(model);

    AttachModel(std::move(model));

    Commit();
}

std::shared_ptr<Mesh> Scene::getMeshWithGeometryID(unsigned id)
{
    for (auto model : this->Models)
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
	float lightInt; // Light intensity
	float fatt = 1 / pow(L.length(), 2); // Factor distancia entre luz y punto
	L = glm::normalize(L);
	lightInt = glm::max(glm::dot(normal, L), 0.f) * fatt * pointLigntInt; // pointLight->getIntensity(); // El dot product de 2 vectores normalizados da el coseno del angulo entre ellos, 50 es la intensidad

	lightInt = glm::clamp(lightInt, 0.0f, 1.0f);
	return lightInt;
}

float computeSpecularPointLightIntensity(
	const glm::vec3& L, const glm::vec3& normal, 
	const Ray& r,
	const float& lightInt, const float& specularExponent, const float& specularFactor
){
	glm::vec3 R = glm::reflect(L, normal);
	float specAmmount = glm::dot(r.Direction, R);
	if (specAmmount < 0) specAmmount = 0;
	return pow(specAmmount, specularExponent) * specularFactor;
}


glm::vec3 Scene::trace(Ray ray, int depth, float currentRefract)
{
	glm::vec3 color = Settings::backgroundColor; 
	if (depth <= 0)
	{
		return color;
	}

	ThrowRay(ray);

	glm::vec3 HitCoordinates;

	if (ray.GetHit(HitCoordinates))
	{
		RTCRayHit* hit = ray.GetRayHit();
		auto mesh = getMeshWithGeometryID(hit->hit.geomID);
		auto material = mesh->getMaterial();

		color += shade(ray, material, Settings::maxDepth, currentRefract);
	}

	return color;
	
}


glm::vec3 Scene::shade(const Ray& r, std::shared_ptr<Material> material, int depth, float currentRefract) {
	// Ambient light
	glm::vec3 color = glm::vec3(0,0,0);
	glm::vec3 normal = r.getNormal();
	glm::vec3 hitPos;
	r.GetHit(hitPos);


	for (auto light : Lights)
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
			ThrowRay(shadowRay);

			if (shadowRay.GetHit(shadowHitPos))
			{
				float opacity = 1;
				RTCRayHit* hit = shadowRay.GetRayHit();
				auto mesh = getMeshWithGeometryID(hit->hit.geomID);
				auto hitMaterial = mesh->getMaterial();
				opacity = hitMaterial->getOpacity();

				if (opacity < 1 || (hitPos - shadowHitPos).length() > (pointLight->getPosition() - shadowHitPos).length())
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

	}

	if (material->getOpacity() < 1) {

		glm::vec3 reflect = glm::reflect(r.Direction, normal);
		glm::vec3 refract = glm::refract(r.Direction, normal, currentRefract / material->getRefraction());

		Ray reflectRay(hitPos, reflect);
		Ray refractRay(hitPos, refract);

		color += trace(reflectRay, depth - 1, material->getRefraction());
		color += trace(refractRay, depth - 1, material->getRefraction());
	}
	else if (material->getReflection())
	{
		glm::vec3 reflect = glm::reflect(r.Direction, normal);
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

