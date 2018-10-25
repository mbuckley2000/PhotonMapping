#pragma once
#include <Eigen/Dense>
#include "Object.h"

class Object;

class Ray
{
public:
	Eigen::Vector3f position;
	Eigen::Vector3f direction;

	bool intersectsWith(Object& object);

	Ray(Eigen::Vector3f position, Eigen::Vector3f direction);
	virtual ~Ray();
};

