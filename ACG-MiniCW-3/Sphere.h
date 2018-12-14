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

	virtual bool rayIntersects(Ray & ray, Object*& o, float& t) override;

	virtual Vec3 getNormalAt(Vec3 position) override;

private:
	float radius;
};