#pragma once
#include "Object.h"
#include <Eigen/Dense>

class Plane :
	public Object
{
public:
	Eigen::Vector3f normal;
	float offset; //Distance from origin

	bool rayIntersects(Ray & ray, float& t, float& u, float& v);
	Eigen::Vector3f getNormalAt(Eigen::Vector3f position);

	Eigen::Vector3f getColour(float u, float v);

	Plane();
	virtual ~Plane();
};

