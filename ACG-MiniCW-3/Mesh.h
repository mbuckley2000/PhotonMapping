#pragma once
#include "Object.h"
#include "Vectors.h"
#include "Triangle.h"
#include "Box.h"

class Mesh :
	public Object
{
public:
	Eigen::MatrixXf vertices;
	Eigen::MatrixXi faces;

	std::vector<Triangle*> triangles;

	void loadFromFile(std::string filename);
	bool rayIntersects(Ray & ray, Object*& o, float& t, float& u, float& v);

	Vec3 getNormalAt(Vec3 position);

	void calculateTriangles();

	Mesh(std::string filename);
	virtual ~Mesh();


private:
	bool trianglesCalculated;
	bool boundingBoxCalculated;
	void calculateBoundingBox();
	Box boundingBox;
};

