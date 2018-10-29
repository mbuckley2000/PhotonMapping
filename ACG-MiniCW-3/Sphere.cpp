#include "pch.h"
#include "Sphere.h"
#include <glm/vec3.hpp>
#include <glm/geometric.hpp>


bool Sphere::rayIntersects(Ray & ray, float& t, float& u, float& v)
{
	const glm::vec3 l =  glm::normalize(ray.direction);
	const glm::vec3 o = ray.position;
	const glm::vec3 c = this->position;
	const float r = this->radius;

	const float value = (pow( glm::dot(l, o-c), 2) - pow(glm::length(o - c), 2) + pow(r, 2));

	return (value >= 0);
}

Sphere::Sphere(float radius)
{
	this->radius = radius;
}

Sphere::~Sphere()
{
}
