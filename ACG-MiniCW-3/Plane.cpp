#include "pch.h"
#include "Plane.h"

#define EPSILON 0.00001

bool Plane::rayIntersects(Ray & ray, Object*& o, float & t, float & u, float & v)
{
	const float denominator = ray.direction.dot(this->normal);

	if (denominator < EPSILON) {
		return false;
	}

	o = this;

	//Intersection
	t = -(ray.position.dot(this->normal) + this->offset) / denominator;

	//Get point
	const Vec3 intersectionPoint = ray.position + (t * ray.direction);
		
	//Get centre of plane
	const Vec3 centre = this->normal * this->offset;
		
	//Get difference between the two
	Vec3 diff = intersectionPoint - centre;
	diff = diff / diff(2);

	u = diff(0);
	v = diff(1);

	return true;

	/*

	float denominator = this->normal.dot(ray.direction);

    if (denominator > EPSILON) {
        const Vec3 difference = this->centre - ray.position;
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

Vec3 Plane::getNormalAt(Vec3 position)
{
	return this->normal;
}

Vec3 Plane::getColour(float u, float v) {
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
