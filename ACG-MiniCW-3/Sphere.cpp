#include "pch.h"
#include "Sphere.h"
#include <Eigen/Dense>

bool Sphere::rayIntersects(Ray & ray, Eigen::Vector3d position)
{
	Eigen::Vector3d l = ray.direction.normalized();
	Eigen::Vector3d o = ray.position;
	Eigen::Vector3d c = position;
	double r = this->radius;

	double value = (pow(l.dot(o - c), 2) - (o - c).squaredNorm() + pow(r, 2));

	return (value >= 0);
}

Sphere::Sphere(double radius)
{
	this->radius = radius;
}

Sphere::~Sphere()
{
}
