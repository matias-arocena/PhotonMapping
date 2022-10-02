#pragma once
#include <embree3/rtcore.h>
#include <memory>
#include <vector>
#include "Model.h"

class Model;
class Ray;

class Scene
{
public:
	Scene(RTCDevice device);
	~Scene();

	RTCScene GetScene();

	void AttachModel(std::shared_ptr<Model> model);
	void Commit();
	void ThrowRay(Ray& Ray);
private:
	RTCScene TheScene;
	RTCIntersectContext context;

	std::vector<std::shared_ptr<Model>> Models;

};

