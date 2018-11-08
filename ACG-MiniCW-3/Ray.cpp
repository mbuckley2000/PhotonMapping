#include "pch.h"
#include "Ray.h"
#include "Object.h"

bool Ray::intersectsWith(Object& object, float& t, float& u, float& v)
{
	return object.rayIntersects(*this, t, u, v);
}

bool Ray::intersectsWith(Scene & scene)
{
	Object* o = (Object*)malloc(sizeof(o));
	float t = INFINITY;
	float u, v;
	const bool hit = this->intersectsWith(scene, o, t, u, v);

	free(o);

	return hit;
}

bool Ray::intersectsWith(Scene & scene, Object* closestObject, float & t, float & u, float & v)
{
	bool found = false;
	float tempT, tempU, tempV;

	for (auto const& object : scene.objects) {
		if (this->intersectsWith(*object, tempT, tempU, tempV)) {
			if (tempT < t && tempT > 0) {
				found = true;
				t = tempT;
				u = tempU;
				v = tempV;
				closestObject = object;
			}
		}
	}

	return found;
}

Ray::Ray(Eigen::Vector3f position, Eigen::Vector3f direction)
{
	this->position = position;
	this->direction = direction;
}


Ray::~Ray()
{
}
