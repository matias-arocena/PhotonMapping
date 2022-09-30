#pragma once
#include <embree3/rtcore.h>
#include <memory>

class Model;

class Scene
{
public:
	Scene(RTCDevice device);
	~Scene();

	RTCScene GetScene();

	void AttachModel(std::unique_ptr<Model> Model);
	void Commit();
private:
	RTCScene TheScene;

};

