#pragma once
#include "Ray.h"
#include <Eigen/Dense>

class Ray;

class Object
{
public:
	Object() {}
	virtual ~Object() {}

	virtual bool rayIntersects(Ray& ray, Eigen::Vector3d position) = 0;
};

