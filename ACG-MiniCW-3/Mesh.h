#pragma once
#include "Object.h"
#include <Eigen/Dense>
#include "Triangle.h"
using namespace Eigen;

class Mesh :
	public Object
{
public:
	MatrixXf vertices;
	MatrixXi faces;
	std::vector<Triangle*> triangles;

	void loadFromFile(std::string filename);
	bool rayIntersects(Ray& ray);
	void calculateTriangles();
	void calculateBoundingSphere();

	Mesh(std::string filename);
	virtual ~Mesh();
private:
	bool trianglesCalculated;
	bool boundingSphereCalculated;
};

