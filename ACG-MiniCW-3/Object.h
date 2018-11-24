#pragma once
#include "Ray.h"
#include "Vectors.h"

class Ray;

class Object
{
public:
	Vec3 position;
	Vec3 colour;
	float specularPower;
	float specularCoeff;
	float ambient;
	bool shadow = false;

	Object() {}
	virtual ~Object() {}

	virtual Vec3 getNormalAt(Vec3 position) = 0;
	virtual bool rayIntersects(Ray & ray, Object*& o, float& t, float& u, float& v) = 0;
	Vec3 getColour(float u, float v) {
		return this->colour;
	};
};

