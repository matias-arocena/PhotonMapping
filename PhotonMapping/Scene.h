#pragma once
#include <embree3/rtcore.h>
#include <memory>
#include <vector>
#include "Light.h"
#include "Camera.h"
#include <iostream>
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
	void addLight(std::shared_ptr<Light> light);
	void setCamera(std::shared_ptr<Camera> camera);
	void saveImage(std::vector<glm::vec3> buffer);
	void addModel(std::string objRoute, glm::vec3 position);

	std::shared_ptr<Mesh> getMeshWithGeometryID(unsigned id);

	std::shared_ptr<Camera> camera;
	RTCDevice device;

private:

	RTCScene TheScene;
	RTCIntersectContext context;
	std::vector<std::shared_ptr<Model>> Models;

	std::vector<std::shared_ptr<Light>> Lights;
};

