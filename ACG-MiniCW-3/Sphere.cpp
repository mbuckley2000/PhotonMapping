#include "pch.h"
#include "Sphere.h"
#include <Eigen/Dense>

bool Sphere::rayIntersects(Ray & ray)
{
	const Eigen::Vector3f l = ray.direction.normalized();
	const Eigen::Vector3f o = ray.position;
	const Eigen::Vector3f c = this->position;
	const float r = this->radius;

	const float value = (pow(l.dot(o - c), 2) - (o - c).squaredNorm() + pow(r, 2));

	return (value >= 0);
}

Sphere::Sphere(float radius)
{
	this->radius = radius;
}

Sphere::~Sphere()
{
}
