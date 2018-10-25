#pragma once
#include "Object.h"
#include <Eigen/Dense>

class Sphere :
	public Object
{
public:
	float radius;

	bool rayIntersects(Ray& ray);

	Sphere(float radius);
	virtual ~Sphere();
};