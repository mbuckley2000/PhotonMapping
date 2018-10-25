#include "pch.h"
#include "Ray.h"
#include "Object.h"


bool Ray::intersectsWith(Object & object)
{
	return object.rayIntersects(*this);
}

Ray::Ray(Eigen::Vector3f position, Eigen::Vector3f direction)
{
	this->position = position;
	this->direction = direction;
}


Ray::~Ray()
{
}
