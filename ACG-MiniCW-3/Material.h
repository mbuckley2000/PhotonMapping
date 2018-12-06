#pragma once
#include "Vectors.h"


class Material
{
public:
	float diffuseProbability = 0.9;
	float specularProbability = 0.3;
	float specularPower = 32;
	float indexOfRefraction = 1;
	bool refractive = false;
	Vec3 baseColour = Vec3(1, 1, 1);

	Material();
	virtual ~Material();
};

