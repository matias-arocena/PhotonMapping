#pragma once
#include <embree3/rtcore.h>
#include <memory>
#include <vector>
#include "Light.h"
#include "Camera.h"
#include <iostream>
#include "Model.h"
#include "PhotonMap.h"
#include "PointLight.h"

class Model;
class Ray;

class Scene
{
public:
	static Scene& getInstance();

	Scene(Scene const&) = delete;
	void operator=(Scene const&) = delete;

	RTCScene getScene();

	void attachModel(std::shared_ptr<Model> model);
	void commit();
	void throwRay(Ray& Ray);
	void addLight(std::shared_ptr<Light> light);
	void setCamera(std::shared_ptr<Camera> camera);
	std::shared_ptr<Camera> getCamera();
	void saveImage(std::vector<glm::vec3> buffer);
	void addModel(std::string objRoute, glm::vec3 position, float reflection, float refraction);

	std::shared_ptr<Mesh> getMeshWithGeometryID(unsigned id);
	bool getMeshWithGeometryID(unsigned id, Mesh& mesh);

	std::vector<glm::vec3> renderScene();


private:
	Scene();
	~Scene();

	RTCScene scene;
	RTCIntersectContext context;
	std::vector<std::shared_ptr<Model>> models;
	std::vector<std::shared_ptr<Light>> lights;
	std::shared_ptr<Camera> camera;

	glm::vec3 trace(Ray ray, int depth, float currentRefract);
	glm::vec3 shade(const Ray& r, std::shared_ptr<Material> material, int depth, float currentRefract);

};

