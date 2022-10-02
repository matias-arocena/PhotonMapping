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

void Scene::AttachModel(std::shared_ptr<Model> model)
{
	std::vector<std::shared_ptr<Mesh>> meshes = model->getMeshes();
	for ( std::shared_ptr<Mesh> mesh : meshes)
	{
		rtcAttachGeometry(TheScene, mesh->GetGeometry());
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

