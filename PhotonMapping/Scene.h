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
	void throwRay(std::shared_ptr<Ray> ray);
	void addLight(std::shared_ptr<Light> light);
	void setCamera(std::shared_ptr<Camera> camera);
	std::shared_ptr<Camera> getCamera();
	void saveImage(std::vector<glm::vec3> buffer, std::string path);
	void addModel(std::string objRoute, glm::vec3 position, float reflection, float refraction);

	std::shared_ptr<Mesh> getMeshWithGeometryID(unsigned id);
	//bool getMeshWithGeometryID(unsigned id, std::shared_ptr<Mesh> mesh);

	std::shared_ptr<PhotonMap> getGlobalPhotonMap();
	void generateGlobalPhotonMap();
	void setGlobalPhotonMap(std::shared_ptr<PhotonMap> photonMap);

	std::shared_ptr<PhotonMap> getCausticPhotonMap();
	void generateCausticPhotonMap();

	std::vector<glm::vec3> renderScene();


private:
	Scene();
	~Scene();

	RTCScene scene;
	RTCIntersectContext context;
	std::vector<std::shared_ptr<Model>> models;
	std::vector<std::shared_ptr<Light>> lights;
	std::shared_ptr<Camera> camera;
	std::vector<std::shared_ptr<Mesh>> meshes;
	std::shared_ptr<PhotonMap> global; 
	std::shared_ptr<PhotonMap> caustic;

	glm::vec3 trace(std::shared_ptr<Ray> ray, const int& depth, const float& currentRefract);
	glm::vec3 shade(std::shared_ptr<Ray> r, std::shared_ptr<Material> material, int depth, float currentRefract);
	glm::vec3 computeShadow(const glm::vec3& lightPosition, const glm::vec3& normal, const glm::vec3& hitPos, const float& intensity, std::shared_ptr<Material> material, std::shared_ptr<Ray> r, const int& lightId);
};

