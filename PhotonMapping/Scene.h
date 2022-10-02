#pragma once
#include <embree3/rtcore.h>
#include <memory>
#include <vector>

class Model;
class Ray;

class Scene
{
public:
	Scene(RTCDevice device);
	~Scene();

	RTCScene GetScene();

	void AttachModel(std::unique_ptr<Model> Model);
	void Commit();
	void ThrowRay(Ray& Ray);
private:
	RTCScene TheScene;
	RTCIntersectContext context;

	std::vector<std::unique_ptr<Model>> Models;
};

