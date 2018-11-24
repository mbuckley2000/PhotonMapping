#pragma once
#include "Light.h"
#include "Vectors.h"
class PointLight :
	public Light
{
public:
	PointLight(Vec3 position, Vec3 colour);
	virtual ~PointLight();

	Vec3 position;
	Vec3 colour;

	// Inherited via Light
	virtual Vec3 vectorTo(Vec3 position) override;

	// Inherited via Light
	virtual float getDistanceFrom(Vec3 position) override;

};

