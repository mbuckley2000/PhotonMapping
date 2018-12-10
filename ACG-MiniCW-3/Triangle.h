#pragma once
#include "Object.h"
#include "Vectors.h"
#include "Box.h"
class Triangle :
	public Object
{
public:
	Triangle(Vec3 v0, Vec3 v1, Vec3 v2);
	Triangle(Mat3 vertices, Mat3 vertexNormals);
	virtual ~Triangle();

	bool rayIntersects(Ray & ray, Object*& o, float& t, float& u, float& v);
	Vec3 getNormalAt(float u, float v);
	Vec3 getNormalAt(Vec3 position);
	Vec3 getVertex(int vertex);
	Box getBoundingBox();
	Vec3 getMidpoint();

private:
	Vec3 edge1;
	Vec3 edge2;
	Vec3 normal;
	Box boundingBox;
	bool boundingBoxCalculated = false;
	Vec3 midPoint;
	bool midPointCalculated = false;
	Mat3 vertices;
	std::vector<Vec3> vertexNormals;
};

