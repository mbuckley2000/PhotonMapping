#pragma once
#include "Object.h"
#include <Eigen/Dense>

class Sphere :
	public Object
{
public:
	float radius;

	bool rayIntersects(Ray& ray, float& t, float& u, float& v);

	Sphere(float radius);
	virtual ~Sphere();
};