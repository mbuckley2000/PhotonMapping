#pragma once
#include "Vectors.h"
typedef struct Photon {
	Vec3 position;
	Vec3 flux;
	Vec3 incomingAngle;
	float squaredDistFromSearchPoint;
} Photon;