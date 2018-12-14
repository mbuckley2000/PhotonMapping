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

	/**
	 * This should not be used and has not been implemented, as boxes are only used for KD-Tree calculations and not for rendering
	 */
	virtual Vec3 getNormalAt(Vec3 position) override;

	/**
	 * Fast Ray-Box Intersection
	 * by Andrew Woo
	 * from "Graphics Gems", Academic Press, 1990
	 */
	virtual bool rayIntersects(Ray & ray, Object*& o, float & t) override;

	/**
	 * Returns the largest dimension (0, 1, 2) of the box
	 */
	int largestDim();

	/**
	 * Expands the box to include the given point
	 */
	void expand(Vec3 v);

	/**
	 * Expands the box to include the given box
	 */
	void expand(Box b);

};