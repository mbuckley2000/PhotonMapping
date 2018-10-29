#pragma once
#include "Ray.h"
#include <glm/vec3.hpp>

class Ray;
class Sphere;

class Object
{
public:
	glm::vec3 position;
	glm::vec3 colour;
	Sphere* boundingSphere;

	Object() {}
	virtual ~Object() {}

	virtual bool rayIntersects(Ray& ray, float& t, float& u, float& v) = 0;
};

