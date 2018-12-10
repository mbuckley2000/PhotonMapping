#pragma once
#include "pch.h"
#include "Vectors.h"

class BoundingBox {
public:
	Vec3 maximum;
	Vec3 minimum;

	BoundingBox();
	void expand(BoundingBox b);
	void expand(Vec3 v);
	int longestDim();
};