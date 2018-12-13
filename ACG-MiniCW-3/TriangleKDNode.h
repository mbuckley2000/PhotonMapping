#pragma once
#include "Vectors.h"
#include "Box.h"
#include "Triangle.h"
#include "Ray.h"
#include "Object.h"

class TriangleKDNode
{
public:
	Box boundingBox;
	TriangleKDNode* left;
	TriangleKDNode* right;
	std::vector<Triangle*> triangles;

	TriangleKDNode();
	TriangleKDNode(std::vector<Triangle*> triangles);
	~TriangleKDNode();

	bool intersect(Ray & ray, Object*& o, float & t, float& u, float& v);

private:
	void balance();
	void expandBoundingBox();
};