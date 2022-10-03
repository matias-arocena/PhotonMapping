#include "Scene.h"

#include <FreeImage.h>
#include "Model.h"
#include "Ray.h"
#include "Settings.h"

#ifdef _DEBUG
#include <iostream>
#endif



Scene::Scene(RTCDevice device)
{
    this->device = device;
	TheScene = rtcNewScene(device);
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


void Scene::addModel(std::string objRoute, glm::vec3 position)
{
    std::shared_ptr<Model> model = std::make_shared<Model>(objRoute.c_str(), device, position);

    Models.push_back(model);

    AttachModel(std::move(model));

    Commit();
}

