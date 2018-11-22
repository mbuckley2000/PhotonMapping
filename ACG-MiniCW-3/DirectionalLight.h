#pragma once
#include "Light.h"
#include "Vectors.h"

class DirectionalLight :
	public Light
{
public:
	Vec3 direction;

	DirectionalLight(Vec3 direction, Vec3 colour);

	Vec3 vectorTo(Vec3 position);

	virtual ~DirectionalLight();
};

