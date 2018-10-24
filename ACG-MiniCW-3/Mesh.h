#pragma once
#include "Object.h"
#include <Eigen/Dense>
#include "Triangle.h"
using namespace Eigen;

class Mesh :
	public Object
{
public:
	MatrixXd vertices;
	MatrixXi faces;
	std::vector<Triangle*> triangles;

	void loadFromFile(std::string filename);
	bool rayIntersects(Ray& ray, Eigen::Vector3d position);
	void calculateTriangles();

	Mesh();
	virtual ~Mesh();
private:
	bool trianglesCalculated;
};

