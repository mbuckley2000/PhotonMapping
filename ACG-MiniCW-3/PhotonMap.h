#pragma once
#include "Vectors.h"
#include "Light.h"
#include "Ray.h"
#include <vector>
#include "Photon.h"
#include "PhotonTree.h"
#include "Scene.h"
#include <random>
#include <mutex>
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

	Ray generatePhotonRay(Light * light, std::default_random_engine* generator, std::uniform_real_distribution<float>* distribution);
	void tracePhoton(Ray * photonRay, Vec3 flux, std::vector<Photon>* photons, std::default_random_engine* generator, std::uniform_real_distribution<float>* distribution);
	void mapperThread(int threadID, int numPhotons, Vec3 photonFlux);

	std::mutex photonMappingMutex;

};


