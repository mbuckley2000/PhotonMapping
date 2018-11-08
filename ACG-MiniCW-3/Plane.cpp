#include "pch.h"
#include "Plane.h"

#define EPSILON 0.00001

bool Plane::rayIntersects(Ray & ray, float & t, float & u, float & v)
{
	const float denominator = ray.direction.dot(this->normal);

	if (denominator > EPSILON) {
		//Intersection
		t = -(ray.position.dot(this->normal) + this->offset) / denominator;

		//Get point
		const Eigen::Vector3f intersectionPoint = ray.position + (t * ray.direction);
		
		//Get centre of plane
		const Eigen::Vector3f centre = this->normal * this->offset;
		
		//Get difference between the two
		Eigen::Vector3f diff = intersectionPoint - centre;
		diff = diff / diff(2);

		u = diff(0);
		v = diff(1);

		return true;
	}

	return false;


	/*

	float denominator = this->normal.dot(ray.direction);

    if (denominator > EPSILON) {
        const Eigen::Vector3f difference = this->centre - ray.position;
        float tempT = difference.dot(this->normal) / denominator;
		t = tempT;
		return true;

        if (tempT > 0.0001) {
			t = tempT;
			return true;
        }
    }

	return false;
	*/
}

Eigen::Vector3f Plane::getNormalAt(Eigen::Vector3f position)
{
	return this->normal;
}

Eigen::Vector3f Plane::getColour(float u, float v) {
	const int uI = u;
	const float dimmingFactor = 0.1;

	if (uI % 2 < 1) {
		return this->colour;
	}
	else {
		return this->colour * dimmingFactor;
	}
};

Plane::Plane()
{
}


Plane::~Plane()
{
}
