#include "pch.h"
#include "Sphere.h"


//https://en.wikipedia.org/wiki/Line%E2%80%93sphere_intersection
bool Sphere::rayIntersects(Ray & ray, Object*& hitObject, float& t)
{
	const Vec3 direction = ray.direction.normalized();
	const Vec3 origin = ray.position;
	const Vec3 centre = this->position;

	const float value = (pow(direction.dot(origin - centre), 2) - (origin - centre).squaredNorm() + pow(this->radius, 2));
	t = 1;
	if (value < 0) {
		//No solutions exist
		return false;
	}

	hitObject = this;

	float part1 = -(direction.dot(origin - centre));

	if (value > 0) {
		//Two solutions exists
		//Calc t
		const float rootVal = sqrtf(value);

		//Solutions for t are part1 +- rootVal. We only want the smaller solution.
		//rootval must be positive as it is the result of sqrt
		t = part1 - rootVal;

		return true;
	}

	//One solution exists
	t = part1;

	return true;
}

Vec3 Sphere::getNormalAt(Vec3 position)
{
	return Vec3(position - this->position).normalized();
}

Sphere::Sphere(float radius)
{
	this->radius = radius;
}

Sphere::~Sphere()
{
}


