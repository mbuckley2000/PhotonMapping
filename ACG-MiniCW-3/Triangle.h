#pragma once
#include "Object.h"

class Triangle :
	public Object
{
public:
	Eigen::Matrix3f vertices;

	bool rayIntersects(Ray& ray);

	Triangle(Eigen::Matrix3f vertices);
	virtual ~Triangle();
};

