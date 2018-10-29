#pragma once
#include <Eigen/Dense>
#include "Object.h"

class Object;

class Ray
{
public:
	Eigen::Vector3f position;
	Eigen::Vector3f direction;

	bool intersectsWith(Object& object, float& t, float& u, float& v);

	Ray(Eigen::Vector3f position, Eigen::Vector3f direction);
	virtual ~Ray();
};

