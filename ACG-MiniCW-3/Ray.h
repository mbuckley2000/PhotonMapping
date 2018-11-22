#pragma once
#include "Object.h"
#include "Scene.h"
#include "Vectors.h"

class Scene;
class Object;

class Ray
{
public:
	Vec3 position;
	Vec3 direction;

	bool intersectsWith(Object& object, Object*& hitObj, float& t, float& u, float& v);
	bool intersectsWith(Scene & scene);
	bool intersectsWith(Scene & scene, Object*& closestObject, float & t, float & u, float & v);

	Ray(Vec3 position, Vec3 direction);
	virtual ~Ray();
};

