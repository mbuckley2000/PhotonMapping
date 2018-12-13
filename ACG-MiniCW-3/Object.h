#pragma once
#include "Material.h"
#include "Vectors.h"

class Ray;

class Object
{
public:
	Vec3 position;
	Material material;
	bool shadow = false;

	Object() {}
	virtual ~Object() {}

	virtual Vec3 getNormalAt(Vec3 position) = 0;
	virtual bool rayIntersects(Ray & ray, Object*& o, float& t, float& u, float& v) = 0;
	Vec3 getColour(float u, float v) {
		return this->material.baseColour;
	};
};

