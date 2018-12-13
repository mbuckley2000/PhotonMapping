#pragma once
#include "Object.h"
#include "Ray.h"
#include "Vectors.h"

class Object;
class Ray;

class Box :
	public Object
{
public:
	Vec3 minimum, maximum;

	Box();
	virtual ~Box();

	// Inherited via Object
	virtual Vec3 getNormalAt(Vec3 position) override;
	virtual bool rayIntersects(Ray & ray, Object*& o, float & t, float & u, float & v) override;

	int largestDim();
	void expand(Vec3 v);
	void expand(Box b);

};

