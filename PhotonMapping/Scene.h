#pragma once
#include <embree3/rtcore.h>
#include <memory>
#include <vector>
#include "Model.h"

class Scene
{
public:
	Scene(RTCDevice device);
	~Scene();

	RTCScene GetScene();

	void AttachModel(std::shared_ptr<Model> model);
	void Commit();
private:
	RTCScene TheScene;

	std::vector<std::shared_ptr<Model>> Models;

};

