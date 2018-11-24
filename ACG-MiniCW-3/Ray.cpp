#include "pch.h"
#include "Ray.h"
#include "Object.h"
#include "Sphere.h"
bool Ray::intersectsWith(Object& object, Object*& hitObj, float& t, float& u, float& v)
{
	return object.rayIntersects(*this, hitObj, t, u, v);
}

bool Ray::intersectsWith(Scene & scene, float& t)
{
	Object* o = NULL;// (Object*)malloc(sizeof(o));
	t = INFINITY;
	float u, v;
	const bool hit = this->intersectsWith(scene, o, t, u, v);

	//free(o);

	return hit;
}

bool Ray::intersectsWith(Scene & scene, Object*& closestObjectPtr, float & t, float & u, float & v)
{
	bool found = false;
	float tempT, tempU, tempV;
	Object* hitObj = NULL;

	t = INFINITY;

	for (auto const& object : scene.objects) {
		if (this->intersectsWith(*object, hitObj, tempT, tempU, tempV)) {
			if (tempT < t && tempT > 0) {
				found = true;
				t = tempT;
				u = tempU;
				v = tempV;
				closestObjectPtr = hitObj;
			}
		}
	}

	return found;
}

Ray::Ray(Vec3 position, Vec3 direction)
{
	this->position = position;
	this->direction = direction;
}


Ray::~Ray()
{
}
