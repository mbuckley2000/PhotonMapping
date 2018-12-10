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

// Comparator for sorting photons based on a given dimension of their position vector
struct CompareDimensions {
	CompareDimensions(int dimension) {
		this->dimension = dimension;
	}

	bool operator () (Triangle* i, Triangle* j) {
		return (i->getMidpoint()(dimension) < j->getMidpoint()(dimension));
	}

	int dimension;
};

//Returns a slice of a vector
//template <typename T>
std::vector<Triangle*> sliceVec(int startPos, int endPos, std::vector<Triangle*> vector) {
	std::vector<Triangle*> out;

	for (int i = startPos; i <= endPos; i++) {
		out.push_back(vector[i]);
	}

	return out;
}

TriangleKDNode * TriangleKDNode::balance(std::vector<Triangle*> triangles, int depth)
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


	if (triangles.size() < 10) {
		node->left = new TriangleKDNode();
		node->right = new TriangleKDNode();
		node->left->triangles = std::vector<Triangle*>();
		node->right->triangles = std::vector<Triangle*>();
		return node;
	}

	Vec3 midPoint(0, 0, 0);
	for (int i = 0; i < triangles.size(); i++) {
		midPoint += triangles[i]->getMidpoint();
	}
	midPoint /= triangles.size();



	//Sort the array just until middle (median) posision is correct.
	//All elems preceding median are less than it, and all following median are greater than it.
	//Much faster than sort() with same eventual outcome (almost 2x speed)
	const int medianArrayPos = triangles.size() / 2;
	const int largestDim = node->boundingBox.largestDim();
	std::nth_element(triangles.begin(), triangles.begin() + medianArrayPos, triangles.end(), CompareDimensions(largestDim));

	//if (triangles.size() > 1) { WE KNOW IT IS >1 already
		//Left tree becomes all photons less than median
	node->left = balance(sliceVec(0, medianArrayPos, triangles), depth + 1);

	if (medianArrayPos + 1 < triangles.size()) {
		//Right tree becomes all photons greater than median
		node->right = balance(sliceVec(medianArrayPos + 1, triangles.size() - 1, triangles), depth + 1);
	}
	else {
		node->right = NULL;
	}
	//}




	//std::vector<Triangle*> leftTriangles;
	//std::vector<Triangle*> rightTriangles;
	//const int axis = node->boundingBox.largestDim();
	//for (int i = 0; i < triangles.size(); i++) {
	//	//Split triangles based on the median midpoint in the longest axis
	//	midPoint[axis] >= triangles[i]->getMidpoint()[axis] ? rightTriangles.push_back(triangles[i]) : leftTriangles.push_back(triangles[i]);
	//}

	//if (leftTriangles.size() == 0 && rightTriangles.size() > 0) {
	//	leftTriangles = rightTriangles;
	//}
	//else if (rightTriangles.size() == 0 && leftTriangles.size() > 0) {
	//	rightTriangles = leftTriangles;
	//}




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
	/*if (leftTriangles.size() > 10) {
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
	}*/
	
	return node;
}


bool TriangleKDNode::intersect(Ray & ray, Object*& o, float & t, float& u, float& v)
{
	//Check to see if the ray intersects the bounding box of the given node

	float boxt, boxu, boxv;
	Object* boxo;

	if (this->boundingBox.rayIntersects(ray, boxo, boxt, boxu, boxv)) {
		bool hit_tri = false;

		if (this->left || this->right) {
			if (this->left->triangles.size() > 0 || this->right->triangles.size() > 0) {
				bool hitLeft = false;
				bool hitRight = false;

				float tl, ul, vl, tr, ur, vr;
				Object* ol;
				Object* orr;

				if (this->left->triangles.size() > 0) {
					hitLeft = this->left->intersect(ray, ol, tl, ul, vl);
				}
				if (this->right->triangles.size() > 0) {
					hitRight = this->right->intersect(ray, orr, tr, ur, vr);
				}

				if (hitLeft && hitRight) {
					if (tl < tr) {
						t = tl;
						u = ul;
						v = vl;
						o = ol;
					}
					else {
						t = tr;
						u = ur;
						v = vr;
						o = orr;
					}
				}
				else if (hitLeft) {
					t = tl;
					u = ul;
					v = vl;
					o = ol;
				}
				else {
					t = tr;
					u = ur;
					v = vr;
					o = orr;
				}
				
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
