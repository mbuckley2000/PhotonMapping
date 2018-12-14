#pragma once
#include "Material.h"
#include "Vectors.h"

class Ray;

class Object
{
public:
	Vec3 position;
	Material material;
	bool shadow = false;

	Object() {}
	virtual ~Object() {}

	/**
	 * Returns the object's normal at the given world space position vector
	 */
	virtual Vec3 getNormalAt(Vec3 position) = 0;

	/**
	 * Returns true if the given ray intersects the object
	 * Object* o will be set to point to the object that is intersected. This is used when intersection a mesh to return the correct triangle
	 * float& t will be set to the distance along the ray of the intersection point
	 */
	virtual bool rayIntersects(Ray & ray, Object*& o, float& t) = 0;
};

