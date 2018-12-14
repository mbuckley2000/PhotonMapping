#pragma once
#include "Vectors.h"


class Material
{
public:
	float diffuseProbability = 0.6;
	float specularProbability = 0.3;
	float indexOfRefraction = 1;
	double* brdf = nullptr;
	bool transmissive = false;
	float reflectiveness;
	float refractiveness;
};

