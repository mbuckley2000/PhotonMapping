#pragma once
#include "Vectors.h"
#include "Light.h"
#include "Ray.h"
#include <vector>
#include "Photon.h"
#include "PhotonTree.h"
#include "Scene.h"
#include <random>

class Scene;
class Ray;

class PhotonMap
{
public:
	PhotonMap(Scene* scene);
	virtual ~PhotonMap();

	void mapPhotons(int numPhotons);

	std::vector<Photon> photons;

	std::priority_queue<Photon*, std::vector<Photon*>, MaximumDistanceCompare> findNearestNeighbours(Vec3 position, int numberOfNeighbours);

private:
	int numPhotons;
	PhotonTree* tree;
	Scene* scene;
	std::default_random_engine generator;
	std::uniform_real_distribution<float> distributiona;
	std::uniform_real_distribution<float> distributionb;

	Ray generatePhotonRay(Light* light);
	void tracePhoton(Ray* photonRay, Vec3 flux);
	void storePhoton(Vec3 position, Vec3 flux, Vec3 incomingAngle);
};


