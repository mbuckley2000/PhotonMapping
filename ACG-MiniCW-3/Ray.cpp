#include "pch.h"
#include "Ray.h"
#include "Object.h"
#include "Sphere.h"
bool Ray::intersectsWith(Object& object, Object*& hitObj, float& t)
{
	return object.rayIntersects(*this, hitObj, t);
}

bool Ray::intersectsWith(Scene & scene, float& t)
{
	Object* o = NULL;// (Object*)malloc(sizeof(o));
	t = INFINITY;
	float u, v;
	const bool hit = this->intersectsWith(scene, o, t);

	return hit;
}

bool Ray::intersectsWith(Scene & scene, Object*& closestObjectPtr, float & t)
{
	bool found = false;
	float tempT;
	Object* hitObj = NULL;

	t = INFINITY;

	for (auto const& object : scene.objects) {
		if (this->intersectsWith(*object, hitObj, tempT)) {
			if (tempT < t && tempT > 0) {
				found = true;
				t = tempT;
				closestObjectPtr = hitObj;
			}
		}
	}

	return found;
}

Ray::Ray(Vec3 position, Vec3 direction)
{
	this->position = position;
	this->direction = direction.normalized();
}
