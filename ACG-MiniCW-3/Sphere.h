#pragma once
#include "Object.h"
#include "Vectors.h"
#include "Ray.h"

class Sphere :
	public Object
{
public:
	Sphere(float radius);
	virtual ~Sphere();

	bool rayIntersects(Ray & ray, Object*& o, float& t, float& u, float& v);

	Vec3 getNormalAt(Vec3 position);

private:
	float radius;
};