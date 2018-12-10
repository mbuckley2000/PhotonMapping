#include "pch.h"
#include "Box.h"


Box::Box()
{
	this->minimum << 0, 0, 0;
	this->maximum << 0, 0, 0;
}


Box::~Box()
{
}

Vec3 Box::getNormalAt(Vec3 position)
{
	return Vec3(1, 1, 1);
}


/*
	Fast Ray-Box Intersection
	by Andrew Woo
	from "Graphics Gems", Academic Press, 1990
*/
bool Box::rayIntersects(Ray & ray, Object*& o, float & t, float & u, float & v)
{
	#define NUMDIM	3
	#define RIGHT	0
	#define LEFT	1
	#define MIDDLE	2

	const Vec3 minB = this->minimum;
	const Vec3 maxB = this->maximum;
	const Vec3 origin = ray.position;
	const Vec3 dir = ray.direction;

	Vec3 coord;

	bool inside = true;
	char quadrant[NUMDIM];
	register int i;
	int whichPlane;
	double maxT[NUMDIM];
	double candidatePlane[NUMDIM];

	/* Find candidate planes; this loop can be avoided if
	rays cast all from the eye(assume perpsective view) */

	for (i = 0; i < NUMDIM; i++) {
		if (origin[i] < minB[i]) {
			quadrant[i] = LEFT;
			candidatePlane[i] = minB[i];
			inside = false;
		}
		else if (origin[i] > maxB[i]) {
			quadrant[i] = RIGHT;
			candidatePlane[i] = maxB[i];
			inside = false;
		}
		else {
			quadrant[i] = MIDDLE;
		}
	}

	/* Ray origin inside bounding box */
	if (inside) {
		coord = origin;
		o = this;
		return true;
	}


	/* Calculate T distances to candidate planes */
	for (i = 0; i < NUMDIM; i++) {
		if (quadrant[i] != MIDDLE && dir[i] != 0.) {
			maxT[i] = (candidatePlane[i] - origin[i]) / dir[i];
		}
		else {
			maxT[i] = -1.;
		}
	}

	/* Get largest of the maxT's for final choice of intersection */
	whichPlane = 0;
	for (i = 1; i < NUMDIM; i++) {
		if (maxT[whichPlane] < maxT[i]) {
			whichPlane = i;
		}
	}

	/* Check final candidate actually inside box */
	if (maxT[whichPlane] < 0.) {
		return false;
	}

	for (i = 0; i < NUMDIM; i++) {
		if (whichPlane != i) {
			coord[i] = origin[i] + maxT[whichPlane] * dir[i];
			if (coord[i] < minB[i] || coord[i] > maxB[i]) {
				return false;
			}
		}
		else {
			coord[i] = candidatePlane[i];
		}
	}

	o = this;
	return true;				/* ray hits box */
}


void Box::expand(Box b)
{
	for (int d = 0; d < 3; d++) {
		if (b.minimum[d] < this->minimum[d]) {
			this->minimum[d] = b.minimum[d];
		}
		if (b.maximum[d] > this->maximum[d]) {
			this->maximum[d] = b.maximum[d];
		}
	}
}

void Box::expand(Vec3 v)
{
	for (int d = 0; d < 3; d++) {
		if (v[d] < this->minimum[d]) {
			this->minimum[d] = v[d];
		}
		if (v[d] > this->maximum[d]) {
			this->maximum[d] = v[d];
		}
	}
}

int Box::largestDim()
{
	float longest = 0;
	int dim = 0;

	for (int d = 0; d < 3; d++) {
		const float length = maximum[d] - minimum[d];
		if (length > longest) {
			longest = length;
			dim = d;
		}
	}

	return dim;
}
