#pragma once
#include "Vectors.h"
#include "Light.h"
#include "Ray.h"
#include <vector>

typedef struct Photon {
	Vec3 position;
	Vec3 flux;
	Vec3 incomingAngle;
} Photon;

class PhotonMap
{
public:
	PhotonMap(Scene* scene);
	virtual ~PhotonMap();

	void mapPhotons(int numPhotons);

	std::vector<Photon> photons;


private:
	int numPhotons;
	Scene* scene;


	Ray generatePhotonRay(Light* light);
	Photon tracePhoton(Ray* photonRay, Vec3 flux);
	void storePhoton(Vec3 position, Vec3 flux, Vec3 incomingAngle);
};


