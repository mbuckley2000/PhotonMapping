#include "pch.h"
#include "PhotonMap.h"
#include "Common.h"
#include "Object.h"
#include <stdlib.h>
#include <algorithm>
#include "PhotonTree.h"
#include "BRDF.h"

PhotonMap::PhotonMap(Scene * scene)
{
	this->scene = scene;
}

PhotonMap::~PhotonMap()
{
	delete this->tree;
}


void PhotonMap::mapperThread(int threadID, int numPhotons, Vec3 photonFlux) {
	std::vector<Photon> photons;
	std::default_random_engine generator;
	std::uniform_real_distribution<float> distributiona(-1.0, 1.0);
	std::uniform_real_distribution<float> distributionb(0.0, 1.0);


	for (int i = 0; i < numPhotons; i++) {
		Ray photonRay = this->generatePhotonRay(this->scene->light, &generator, &distributiona);
		tracePhoton(&photonRay, photonFlux, &photons, &generator, &distributionb);
	}

	//Mutex lock
	std::lock_guard<std::mutex> guard(this->photonMappingMutex);

	//Copy to global array
	for (int i = 0; i < photons.size(); i++) {
		this->photons.push_back(photons[i]);
	}

	//Mutex unlocks by going out of scope
}


void PhotonMap::mapPhotons(int numPhotons)
{
	Light* light = this->scene->light;

	const Vec3 photonFlux = light->colour / numPhotons;

	std::vector<std::thread> threads;

	std::cout << "Kicking off photon mapping threadss" << std::endl;
	const int numThreads = 12;
	for (int i = 0; i < numThreads; i++) {
		threads.push_back(std::thread(&PhotonMap::mapperThread, this, i, numPhotons/numThreads, photonFlux));
	}

	for (int i = 0; i < numThreads; i++) {
		threads[i].join();
	}
	std::cout << "Threads have finished" << std::endl;

	std::vector<Photon*> photonPtrs;
	for (auto& photon : this->photons) {
		photonPtrs.push_back(&photon);
	}

	std::cout << "Building photon tree" << std::endl;
	this->tree = new PhotonTree(photonPtrs);
}

std::priority_queue<Photon*, std::vector<Photon*>, MaximumDistanceCompare> PhotonMap::findNearestNeighbours(Vec3 position, int numberOfNeighbours)
{
	std::priority_queue<Photon*, std::vector<Photon*>, MaximumDistanceCompare> neighbours = this->tree->findNearestNeighbours(position, numberOfNeighbours);
	return neighbours;
}

Ray PhotonMap::generatePhotonRay(Light * light, std::default_random_engine* generator, std::uniform_real_distribution<float>* distribution)
{
	//TODO Need to implement direction masking to prevent wasted photons

	float x, y, z;
	do {
		x = (*distribution)(*generator);
		y = (*distribution)(*generator);
		z = (*distribution)(*generator);
	} while (pow(x, 2) + pow(y, 2) + pow(z, 2) > 1);

	const Vec3 direction = Vec3(x, y, z).normalized();

	return Ray(light->getPosition(), direction);
}

void storePhoton(Vec3 position, Vec3 flux, Vec3 incomingAngle, std::vector<Photon>* photons)
{
	//Store the photon
	Photon p;
	p.position = position;
	p.flux = flux;
	p.incomingAngle = incomingAngle;
	photons->push_back(p);
}

void PhotonMap::tracePhoton(Ray * photonRay, Vec3 flux, std::vector<Photon>* photons, std::default_random_engine* generator, std::uniform_real_distribution<float>* distribution)
{
	//Trace the photon ray
	float t, u, v;
	Object* collisionObj;

	if (photonRay->intersectsWith(*(this->scene), collisionObj, t, u, v)) {
		const Vec3 intersectionPoint = photonRay->position + (photonRay->direction * t);
		
		//Calculate changes
		float r = (*distribution)(*generator); //Ideally use an evenly distributed function such as drand48() on linux. Open source so we can reimplement

		//Chance of diffuse and specular reflections is based on material
		const float sProb = collisionObj->material.specularProbability;
		const float dProb = collisionObj->material.diffuseProbability;

		bool reflected = false;
		bool refracted = false;
		//remove
		
		const Vec3 reflectedVector = reflectVector(-photonRay->direction, collisionObj->getNormalAt(intersectionPoint)).normalized();

		if (r < dProb) {
			//Diffusely reflected
			storePhoton(intersectionPoint, flux, photonRay->direction, photons);
			flux = flux.cwiseProduct(our_getBDRF(photonRay->direction.normalized(), reflectedVector, collisionObj->getNormalAt(intersectionPoint), collisionObj->material.brdf));
			reflected = true;
		} else if (r < dProb + sProb) { //Between dProb and sProb
			//Specular
			r = (*distribution)(*generator);
			if (r < collisionObj->material.reflectiveness) {
				reflected = true;
			} else {
				refracted = true;
			}
		} else {
			//Absorbed
			storePhoton(intersectionPoint, flux, photonRay->direction, photons);
		}

		if (reflected) {
			//Reflect and trace new ray
			photonRay->position = intersectionPoint;
			photonRay->direction = reflectedVector;
			photonRay->position += 0.00001 * photonRay->direction; //To avoid rounding errors
			this->tracePhoton(photonRay, flux, photons, generator, distribution);
		} else if (refracted) {
			photonRay->position = intersectionPoint;
			photonRay->direction = refractVector(photonRay->direction, collisionObj->getNormalAt(intersectionPoint), collisionObj->material.indexOfRefraction);
			photonRay->position += 0.00001 * photonRay->direction; //To avoid rounding errors
			this->tracePhoton(photonRay, flux, photons, generator, distribution);
		}
	}
}