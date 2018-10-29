#include "pch.h"
#include "Ray.h"
#include "Object.h"
#include <glm/vec3.hpp>

bool Ray::intersectsWith(Object & object, float& t, float& u, float& v)
{
	return object.rayIntersects(*this, t, u, v);
}

Ray::Ray(glm::vec3 position, glm::vec3 direction)
{
	this->position = position;
	this->direction = direction;
}


Ray::~Ray()
{
}
