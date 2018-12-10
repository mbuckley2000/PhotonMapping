#pragma once
#include "Object.h"
#include "Vectors.h"
#include "Triangle.h"
#include "Box.h"
#include "TriangleKDNode.h"

class TriangleKDNode;

class Mesh :
	public Object
{
public:
	Eigen::MatrixXf vertices;
	Eigen::MatrixXf vertexNormals;
	Eigen::MatrixXi faces;
	Eigen::MatrixXi faceVns;//   FN  #F list of face indices into vertex normals

	float scale;
	std::vector<Triangle*> triangles;

	void loadFromFile(std::string filename);
	bool rayIntersects(Ray & ray, Object*& o, float& t, float& u, float& v);

	Vec3 getNormalAt(Vec3 position);

	void calculateTriangles();
	void calculateBoundingBox();

	void buildKDTree();

	Mesh(std::string filename);
	virtual ~Mesh();


private:
	bool trianglesCalculated;
	bool boundingBoxCalculated;
	Box boundingBox;
	TriangleKDNode* kdTree;
};

