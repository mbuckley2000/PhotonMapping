#pragma once
#include "Vectors.h"
#include "Light.h"
#include "Ray.h"
#include "Box.h"
#include <vector>
#include "Photon.h"

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
	void tracePhoton(Ray* photonRay, Vec3 flux);
	void storePhoton(Vec3 position, Vec3 flux, Vec3 incomingAngle);
};


