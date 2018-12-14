#include "pch.h"
#include "PhotonMap.h"
#include "Common.h"
#include "Object.h"
#include <stdlib.h>
#include <algorithm>
#include "PhotonTree.h"
#include "BRDF.h"
#include "Triangle.h"

PhotonMap::PhotonMap(Scene * scene, bool caustics, bool aim, Vec3 aimAt)
{
	this->scene = scene;
	this->caustics = caustics;
	this->aiming = aim;
	this->target = aimAt;
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
		tracePhoton(&photonRay, photonFlux, &photons, &generator, &distributionb, 0);
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
	const int numThreads = 8;
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
	const Vec3 pos = light->getPosition();
	Vec3 direction;

	//TODO Need to implement direction masking to prevent wasted photons
	if (this->aiming) {
		const float mult = 0.2;
		const Vec3 randVec((*distribution)(*generator)*mult, (*distribution)(*generator)*mult, (*distribution)(*generator)*mult);
		direction = ((this->target - pos).normalized() + randVec).normalized();
	}
	else {
		float x, y, z;
		do {
			x = (*distribution)(*generator);
			y = (*distribution)(*generator);
			z = (*distribution)(*generator);
		} while (pow(x, 2) + pow(y, 2) + pow(z, 2) > 1);

		direction = Vec3(x, y, z).normalized();
	}
	

	return Ray(pos, direction);
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

void PhotonMap::tracePhoton(Ray * photonRay, Vec3 flux, std::vector<Photon>* photons, std::default_random_engine* generator, std::uniform_real_distribution<float>* distribution, bool caustic)
{
	//Trace the photon ray
	float t;
	Object* collisionObj;

	if (photonRay->intersectsWith(*(this->scene), collisionObj, t)) {
		const Vec3 intersectionPoint = photonRay->position + (photonRay->direction * t);
		const Vec3 incomingVector = photonRay->direction.normalized();

		Vec3 objNormal = collisionObj->getNormalAt(intersectionPoint);

		const Vec3 reflectedVector = reflectVector(-incomingVector, objNormal).normalized();

		//Calculate changes
		float r = (*distribution)(*generator); //Ideally use an evenly distributed function such as drand48() on linux. Open source so we can reimplement

		//Chance of diffuse and specular reflections is based on material
		const float sProb = collisionObj->material.specularProbability;
		const float dProb = collisionObj->material.diffuseProbability;

		bool reflected = false;
		bool refracted = false;	

		if (r < dProb) {
			//Diffusely reflected
			if (caustic || !this->caustics) { //Only store caustics in a caustic map
				storePhoton(intersectionPoint, flux, incomingVector, photons);
			}
			flux = flux.cwiseProduct(our_getBDRF(incomingVector, reflectedVector, objNormal, collisionObj->material.brdf).normalized());
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
			if (caustic || !this->caustics) {//Only store caustics in a caustic map
				storePhoton(intersectionPoint, flux, incomingVector, photons);
			}
		}

		if (reflected) {
			//Reflect and trace new ray
			photonRay->position = intersectionPoint;
			photonRay->direction = -reflectedVector;
			photonRay->position += 0.00001 * photonRay->direction; //To avoid rounding errors
			this->tracePhoton(photonRay, flux, photons, generator, distribution, caustic);
		} else if (refracted) {
			if (this->caustics) {
				photonRay->position = intersectionPoint;
				photonRay->direction = refractVector(incomingVector, collisionObj->getNormalAt(intersectionPoint), collisionObj->material.indexOfRefraction);
				photonRay->position += 0.00001 * photonRay->direction; //To avoid rounding errors
				this->tracePhoton(photonRay, flux, photons, generator, distribution, true);
			}
		}
	}
}