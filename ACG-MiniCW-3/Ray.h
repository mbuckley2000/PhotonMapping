#pragma once
#include <Eigen/Dense>
#include "ObjectInstance.h"

class ObjectInstance;
class Ray;

class Ray
{
public:
	Eigen::Vector3d position;
	Eigen::Vector3d direction;

	bool intersectsWith(ObjectInstance& objectInstance);

	Ray(Eigen::Vector3d position, Eigen::Vector3d direction);
	virtual ~Ray();
};

