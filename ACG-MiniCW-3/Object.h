#pragma once
#include "Ray.h"
#include <Eigen/Dense>

class Ray;

class Object
{
public:
	Eigen::Vector3f position;
	Eigen::Vector3f colour;
	float specularPower;
	float specularCoeff;
	float ambient;

	Object() {}
	virtual ~Object() {}

	virtual Eigen::Vector3f getNormalAt(Eigen::Vector3f position) = 0;
	virtual bool rayIntersects(Ray & ray, Object*& o, float& t, float& u, float& v) = 0;
	Eigen::Vector3f getColour(float u, float v) {
		return this->colour;
	};
};

