#pragma once
#include "Object.h"
#include "Vectors.h"

class Triangle :
	public Object
{
public:
	Triangle(Vec3 v0, Vec3 v1, Vec3 v2);
	Triangle(Mat3 vertices);
	virtual ~Triangle();

	bool rayIntersects(Ray & ray, Object*& o, float& t, float& u, float& v);
	Vec3 getNormalAt(Vec3 position);	
	Vec3 getVertex(int vertex);
private:
	Vec3 edge1;
	Vec3 edge2;
	Vec3 normal;
	Mat3 vertices;
};

