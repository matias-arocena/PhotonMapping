#include "Scene.h"

#include "Model.h"
#include "Ray.h"

#ifdef _DEBUG
#include <iostream>
#endif

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

