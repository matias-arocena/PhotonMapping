#include "Scene.h"

#include <FreeImage.h>
#include "Model.h"
#include "Ray.h"
#include "Settings.h"

#ifdef _DEBUG
#include <iostream>
#endif



void saveImage(std::vector<glm::vec3> buffer, std::string path)
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

Scene::Scene(RTCDevice device)
{
#ifdef _DEBUG
	std::cout << "Created Scene" << std::endl;
#endif

	TheScene = rtcNewScene(device);
	rtcInitIntersectContext(&context);
}

Scene::~Scene()
{
	rtcReleaseScene(TheScene);

#ifdef _DEBUG
	std::cout << "Deleted Scene" << std::endl;
#endif

}

RTCScene Scene::GetScene()
{
	return TheScene;
}

void Scene::AttachModel(std::unique_ptr<Model> Model)
{
	rtcAttachGeometry(TheScene, Model->GetGeometry());
}

void Scene::Commit()
{
	rtcCommitScene(TheScene);
}

void Scene::ThrowRay(Ray& Ray)
{
	rtcIntersect1(TheScene, &context, Ray.GetRayHit());
}

