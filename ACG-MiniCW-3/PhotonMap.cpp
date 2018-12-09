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
	this->distributiona = std::uniform_real_distribution<float>(-1.0, 1.0);
	this->distributionb = std::uniform_real_distribution<float>(0.0, 1.0);

}

PhotonMap::~PhotonMap()
{
	delete this->tree;
}

void PhotonMap::mapPhotons(int numPhotons)
{
	Light* light = this->scene->light;

	const Vec3 photonFlux = light->colour / numPhotons;

	for (int i = 0; i < numPhotons; i++) {
		Ray photonRay = this->generatePhotonRay(light);
		tracePhoton(&photonRay, photonFlux);
	}

	std::vector<Photon*> photonPtrs;
	for (auto& photon : this->photons) {
		photonPtrs.push_back(&photon);
	}

	this->tree = new PhotonTree(photonPtrs);
}

std::priority_queue<Photon*, std::vector<Photon*>, MaximumDistanceCompare> PhotonMap::findNearestNeighbours(Vec3 position, int numberOfNeighbours)
{
	std::priority_queue<Photon*, std::vector<Photon*>, MaximumDistanceCompare> neighbours = this->tree->findNearestNeighbours(position, numberOfNeighbours);
	return neighbours;
}

Ray PhotonMap::generatePhotonRay(Light * light)
{
	//TODO Need to implement direction masking to prevent wasted photons

	float x, y, z;
	do {
		x = this->distributiona(this->generator);
		y = this->distributiona(this->generator);
		z = this->distributiona(this->generator);
	} while (pow(x, 2) + pow(y, 2) + pow(z, 2) > 1);

	const Vec3 direction = Vec3(x, y, z).normalized();

	return Ray(light->getPosition(), direction);
}

void PhotonMap::tracePhoton(Ray * photonRay, Vec3 flux)
{
	//Trace the photon ray
	float t, u, v;
	Object* collisionObj;

	if (photonRay->intersectsWith(*(this->scene), collisionObj, t, u, v)) {
		const Vec3 intersectionPoint = photonRay->position + (photonRay->direction * t);
		
		//Calculate changes
		float r = this->distributionb(this->generator); //Ideally use an evenly distributed function such as drand48() on linux. Open source so we can reimplement

		//Chance of diffuse and specular reflections is based on material
		const float sProb = collisionObj->material.specularProbability;
		const float dProb = collisionObj->material.diffuseProbability;

		bool reflected = false;
		bool refracted = false;
		//remove
		
		const Vec3 reflectedVector = reflectVector(-photonRay->direction, collisionObj->getNormalAt(intersectionPoint)).normalized();

		if (r < dProb) {
			//Diffusely reflected
			this->storePhoton(intersectionPoint, flux, photonRay->direction);
			flux = flux.cwiseProduct(our_getBDRF(photonRay->direction.normalized(), reflectedVector, collisionObj->getNormalAt(intersectionPoint), collisionObj->material.brdf));
			reflected = true;
		} else if (r < dProb + sProb) { //Between dProb and sProb
			//Specular
			r = this->distributionb(this->generator);
			if (r < collisionObj->material.reflectiveness) {
				reflected = true;
			} else {
				refracted = true;
			}
		} else {
			//Absorbed
			this->storePhoton(intersectionPoint, flux, photonRay->direction);
		}

		if (reflected) {
			//Reflect and trace new ray
			photonRay->position = intersectionPoint;
			photonRay->direction = reflectedVector;
			photonRay->position += 0.00001 * photonRay->direction; //To avoid rounding errors
			this->tracePhoton(photonRay, flux);
		} else if (refracted) {
			photonRay->position = intersectionPoint;
			photonRay->direction = refractVector(photonRay->direction, collisionObj->getNormalAt(intersectionPoint), collisionObj->material.indexOfRefraction);
			photonRay->position += 0.00001 * photonRay->direction; //To avoid rounding errors
			this->tracePhoton(photonRay, flux);
		}
	}
}

void PhotonMap::storePhoton(Vec3 position, Vec3 flux, Vec3 incomingAngle)
{
	//Store the photon
	Photon p;
	p.position = position;
	p.flux = flux;
	p.incomingAngle = incomingAngle;
	this->photons.push_back(p);
}

bool pointSphereIntersection(Vec3 point, Vec3 centre, float radius) {
	return ((point - centre).squaredNorm() < pow(radius, 2));
}

std::vector<Photon*> searchMap(Vec3 position, float maxDistance) {
	return std::vector<Photon*>();
}