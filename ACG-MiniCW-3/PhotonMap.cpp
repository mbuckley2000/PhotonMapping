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
}

PhotonMap::~PhotonMap()
{
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

	PhotonTree photonTree = PhotonTree(photonPtrs);
}

Ray PhotonMap::generatePhotonRay(Light * light)
{
	//Need to implement direction masking to prevent wasted photons

	float x, y, z;
	do {
		x = random(-1, 1);
		y = random(-1, 1);
		z = random(-1, 1);
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
		const float r = random(0, 1); //Ideally use an evenly distributed function such as drand48() on linux. Open source so we can reimplement

		//Chance of diffuse and specular reflections is based on material
		const float sProb = collisionObj->material.specularProbability;
		const float dProb = collisionObj->material.diffuseProbability;

		bool reflected = false;

		if (r < dProb) {
			//Diffusely reflected
			this->storePhoton(intersectionPoint, flux, photonRay->direction);
			flux = flux.cwiseProduct(collisionObj->getColour(u, v));
			reflected = true;
		}
		else if (r < dProb + sProb) { //Between dProb and sProb
			//Specularly reflected
			reflected = true;
		}
		else {
			//Absorbed
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