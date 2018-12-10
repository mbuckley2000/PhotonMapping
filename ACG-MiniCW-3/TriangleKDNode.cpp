#include "pch.h"
#include "TriangleKDNode.h"

/*
KD tree for storing HBV for meshes
Code from https://blog.frogslayer.com/kd-trees-for-faster-ray-tracing-with-triangles/
*/


TriangleKDNode::TriangleKDNode()
{
}


TriangleKDNode::~TriangleKDNode()
{
}

TriangleKDNode * TriangleKDNode::balance(std::vector<Triangle*>& triangles, int depth)
{
	TriangleKDNode* node = new TriangleKDNode();
	node->triangles = triangles;
	node->left = NULL;
	node->right = NULL;
	node->boundingBox = Box();

	if (triangles.size() == 0) {
		return node;
	}

	if (triangles.size() == 1) {
		node->boundingBox = triangles[0]->getBoundingBox();
		node->left = new TriangleKDNode();
		node->right = new TriangleKDNode();
		node->left->triangles = std::vector<Triangle*>();
		node->right->triangles = std::vector<Triangle*>();
		return node;
	}

	//Get bounding box surrounding all triangles
	node->boundingBox = triangles[0]->getBoundingBox();

	for (int i = 1; i < triangles.size(); i++) {
		node->boundingBox.expand(triangles[i]->getBoundingBox());
	}

	Vec3 midPoint(0, 0, 0);
	for (int i = 0; i < triangles.size(); i++) {
		midPoint += triangles[i]->getMidpoint();
	}
	midPoint /= triangles.size();

	std::vector<Triangle*> leftTriangles;
	std::vector<Triangle*> rightTriangles;
	const int axis = node->boundingBox.longestDim();
	for (int i = 0; i < triangles.size(); i++) {
		//Split triangles based on the median midpoint in the longest axis
		midPoint[axis] >= triangles[i]->getMidpoint()[axis] ? rightTriangles.push_back(triangles[i]) : leftTriangles.push_back(triangles[i]);
	}

	if (leftTriangles.size() == 0 && rightTriangles.size() > 0) {
		leftTriangles = rightTriangles;
	}
	else if (rightTriangles.size() == 0 && leftTriangles.size() > 0) {
		rightTriangles = leftTriangles;
	}

	//node->left = balance(leftTriangles, depth + 1);
	//node->right = balance(rightTriangles, depth + 1);

	
	////If 50% of triangles match, don't subdivide anymore
	//int matches = 0;
	//for (int i = 0; i < leftTriangles.size(); i++) {
	//	for (int j = 0; j < rightTriangles.size(); j++) {
	//		if (leftTriangles[i] == rightTriangles[j]) {
	//			matches++;
	//		}
	//	}
	//}

//Recurse down left and right sides
	if (leftTriangles.size() > 10) {
		node->left = balance(leftTriangles, depth + 1);
	}
	else {
		node->left = new TriangleKDNode();
		node->left->triangles = std::vector<Triangle*>();
	}

	if (rightTriangles.size() > 10) {
		node->right = balance(rightTriangles, depth + 1);
	}
	else {
		node->right = new TriangleKDNode();
		node->right->triangles = std::vector<Triangle*>();
	}
	
	return node;
}


bool TriangleKDNode::intersect(Ray & ray, Object*& o, float & t, float& u, float& v)
{
	//Check to see if the ray intersects the bounding box of the given node
	if (this->boundingBox.rayIntersects(ray, o, t, u, v)) {
		bool hit_tri = false;

		//If either child has triangles, rcurse down both sides and check for intersections
		if (this->left && this->right) {
			if (this->left->triangles.size() > 0 || this->right->triangles.size() > 0) {
				bool hitLeft = this->left->intersect(ray, o, t, u, v);
				bool hitRight = this->right->intersect(ray, o, t, u, v);
				return hitLeft || hitRight;
			}
			else {
				//We're at a leaf
				for (int i = 0; i < this->triangles.size(); i++) {
					float minT = INFINITY;
					float tempT, tempU, tempV;
					Object* tempO;
					//Check for hit
					if (this->triangles[i]->rayIntersects(ray, tempO, tempT, tempU, tempV)) {
						hit_tri = true;
						if (tempT < minT) {
							minT = tempT;
							t = tempT;
							u = tempU;
							v = tempV;
							o = tempO;
						}
					}
				}

				return hit_tri;
			}
		}
	}

	return false;
}
