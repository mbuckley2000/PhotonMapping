#pragma once
#include "Object.h"
#include <Eigen/Dense>

class Sphere :
	public Object
{
public:
	double radius;

	bool rayIntersects(Ray& ray, Eigen::Vector3d position);

	Sphere(double radius);
	virtual ~Sphere();
};