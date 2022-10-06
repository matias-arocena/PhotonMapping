#include <embree3/rtcore.h>
#include <limits>
#include <iostream>
#include <FreeImage.h>

#include "Camera.h"
#include "Scene.h"
#include "Model.h"
#include "Ray.h"
#include "Settings.h"
#include "Loader.h"
#include "KdTree.h"
#include "PhotonMap.h"
#include <assimp/importer.hpp>



int main()
{
	RTCDevice device = rtcNewDevice(nullptr);

	std::shared_ptr<Scene> scene = std::make_shared<Scene>(device);

	importScene(scene);

	std::vector<glm::vec3> buffer = scene->renderScene();
	scene->saveImage(buffer);

	rtcReleaseDevice(scene->device);

#ifdef _DEBUG
	KdTree tree1 = KdTree::LoadKdTreeFromFile("Assets/kdtreeload.txt");

	std::vector<Photon> photons; 
	Photon p1;
	p1.position.x = 9.3f;
	p1.position.y = 9.4f;
	p1.position.z = 9.6f;
	p1.power[0] = 'A';
	p1.power[1] = 'B';
	p1.power[2] = 'C';
	p1.power[3] = 'D';
	p1.phi = 'P';
	p1.theta = 'T';
	photons.push_back(p1);

	Photon p2;
	p2.position.x = 8.3f;
	p2.position.y = 8.4f;
	p2.position.z = 8.6f;
	p2.power[0] = '1';
	p2.power[1] = '2';
	p2.power[2] = '3';
	p2.power[3] = '4';
	p2.phi = '5';
	p2.theta = '6';
	photons.push_back(p2);

	KdTree tree2;
	tree2.setPoints(&photons.data()[0], 2);
	
	tree2.SaveKdTreeToFile("Assets/kdtreesave.txt");
#endif
}