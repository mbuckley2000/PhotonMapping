#include "pch.h"
#include "Ray.h"


bool Ray::intersectsWith(ObjectInstance & objectInstance)
{
	return objectInstance.rayIntersects(*this);
}

Ray::Ray(Eigen::Vector3d position, Eigen::Vector3d direction)
{
	this->position = position;
	this->direction = direction;
}


Ray::~Ray()
{
}
