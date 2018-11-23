#pragma once
#include "Light.h"
#include "Vectors.h"

class SquareLight :
	public Light
{
public:
	Vec3 position;
	Vec3 normal;
	float size;

	SquareLight(Vec3 colour, Vec3 position, Vec3 normal, float size);
	virtual ~SquareLight();

	// Inherited via Light
	virtual Vec3 vectorTo(Vec3 position) override;
};

