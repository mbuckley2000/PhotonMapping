#include "pch.h"
#include "Ray.h"
#include "Object.h"


bool Ray::intersectsWith(Object& object, float& t, float& u, float& v)
{
	return object.rayIntersects(*this, t, u, v);
}

Ray::Ray(Eigen::Vector3f position, Eigen::Vector3f direction)
{
	this->position = position;
	this->direction = direction;
}


Ray::~Ray()
{
}
