#pragma once
#include <embree3/rtcore.h>
#include <memory>
#include <vector>
#include "Light.h"
#include "Camera.h"
#include <iostream>

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
	void addLight(std::shared_ptr<Light> light);
	void setCamera(std::shared_ptr<Camera> camera);
	void saveImage(std::vector<glm::vec3> buffer);

private:
	RTCScene TheScene;
	RTCIntersectContext context;
	std::shared_ptr<Camera> camera;

	std::vector<std::shared_ptr<Model>> Models;

	std::vector<std::shared_ptr<Light>> Lights;
};

