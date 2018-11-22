#pragma once
#include "Object.h"
#include "Vectors.h"

class Plane :
	public Object
{
public:
	Vec3 normal;
	float offset; //Distance from origin

	bool rayIntersects(Ray & ray, Object*& o, float& t, float& u, float& v);
	Vec3 getNormalAt(Vec3 position);

	Vec3 getColour(float u, float v);

	Plane();
	virtual ~Plane();
};

