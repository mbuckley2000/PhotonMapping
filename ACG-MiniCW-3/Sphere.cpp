#include "pch.h"
#include "Sphere.h"
#include <Eigen/Dense>

//https://en.wikipedia.org/wiki/Line%E2%80%93sphere_intersection
bool Sphere::rayIntersects(Ray & ray, float& t, float& u, float& v)
{
	const Eigen::Vector3f l = ray.direction.normalized();
	const Eigen::Vector3f o = ray.position;
	const Eigen::Vector3f c = this->position;
	const float r = this->radius;

	const float value = (pow(l.dot(o - c), 2) - (o - c).squaredNorm() + pow(r, 2));
	t = 1;
	if (value < 0) {
		//No solutions exist
		return false;
	}

	float part1 = -(l.dot(o - c));

	if (value > 0) {
		//Two solutions exists
		//Calc t
		float rootVal = sqrtf(value);
		//Solutions for t are part1 +- rootVal. We only want the smaller solution.
		float t1 = part1 + rootVal;
		float t2 = part1 - rootVal;

		if (t2 < t1) {
			t = t2;
		}
		else {
			t = t1;
		}

		return true;
	}

	//One solution exists
	t = part1;

	return true;
}

Eigen::Vector3f Sphere::getNormalAt(Eigen::Vector3f position)
{
	return Eigen::Vector3f(position - this->position).normalized();
}

Sphere::Sphere(float radius)
{
	this->radius = radius;
}

Sphere::~Sphere()
{
}
