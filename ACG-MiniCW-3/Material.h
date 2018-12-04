#pragma once
#include "Vectors.h"


class Material
{
public:
	float diffuseProbability = 0.3;
	float specularProbability = 0.3;
	float specularPower = 32;
	Vec3 baseColour = Vec3(1, 1, 1);

	Material();
	virtual ~Material();
};

