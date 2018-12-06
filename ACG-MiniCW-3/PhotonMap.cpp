#include "pch.h"
#include "PhotonMap.h"
#include "Common.h"
#include "Object.h"
#include <stdlib.h>
#include <algorithm>
#include "PhotonTree.h"

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
	//std::vector<Photon*> output;

	std::priority_queue<Photon*, std::vector<Photon*>, MaximumDistanceCompare> neighbours = this->tree->findNearestNeighbours(position, numberOfNeighbours);

	/*assert(neighbours.size() == numberOfNeighbours);

	while (neighbours.size()) {
		output.push_back(neighbours.top());
		neighbours.pop();
	}*/

	return neighbours;
}

Ray PhotonMap::generatePhotonRay(Light * light)
{
	//Need to implement direction masking to prevent wasted photons

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
	//REMOVE THIS
	//flux = flux.normalized();

	//Trace the photon ray
	float t, u, v;
	Object* collisionObj;

	if (photonRay->intersectsWith(*(this->scene), collisionObj, t, u, v)) {
		const Vec3 intersectionPoint = photonRay->position + (photonRay->direction * t);
		
		//Calculate changes
		const float r = this->distributionb(this->generator); //Ideally use an evenly distributed function such as drand48() on linux. Open source so we can reimplement

		//Chance of diffuse and specular reflections is based on material
		const float sProb = collisionObj->material.specularProbability;
		const float dProb = collisionObj->material.diffuseProbability;

		bool reflected = false;
		//remove
		
		if (r < dProb) {
			//Diffusely reflected
			flux = flux.cwiseProduct(collisionObj->getColour(u, v));
			this->storePhoton(intersectionPoint, flux, photonRay->direction);
			reflected = true;
		}
		else if (r < dProb + sProb) { //Between dProb and sProb
			//Specularly reflected
			reflected = true;
		}
		else {
			//Absorbed
			//flux = flux.cwiseProduct(collisionObj->getColour(u, v));
			this->storePhoton(intersectionPoint, flux, photonRay->direction);
		}

		if (reflected) {
			//Reflect and trace new ray
			photonRay->position = intersectionPoint;
			photonRay->direction = reflectVector(-photonRay->direction, collisionObj->getNormalAt(intersectionPoint));
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