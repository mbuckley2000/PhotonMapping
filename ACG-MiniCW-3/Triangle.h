#pragma once
#include "Object.h"

class Triangle :
	public Object
{
public:
	Eigen::Matrix3f vertices;

	bool rayIntersects(Ray & ray, float& t, float& u, float& v);
	Eigen::Vector3f getNormalAt(Eigen::Vector3f position);

	Triangle(Eigen::Matrix3f vertices);
	virtual ~Triangle();

private:
	Eigen::Vector3f edge1;
	Eigen::Vector3f edge2;
	Eigen::Vector3f normal;

};

