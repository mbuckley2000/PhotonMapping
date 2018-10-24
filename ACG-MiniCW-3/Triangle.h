#pragma once
#include "Object.h"

class Triangle :
	public Object
{
public:
	Eigen::Matrix3d vertices;

	bool rayIntersects(Ray& ray, Eigen::Vector3d position);

	Triangle(Eigen::Matrix3d vertices);
	virtual ~Triangle();
};

