#pragma once
#include "Ray.h"
#include <Eigen/Dense>

class Ray;
class Sphere;

class Object
{
public:
	Eigen::Vector3f position;
	Eigen::Vector3f colour;
	float specular;
	float ambient;

	Sphere* boundingSphere;

	Object() {}
	virtual ~Object() {}

	virtual Eigen::Vector3f getNormalAt(Eigen::Vector3f position) = 0;
	virtual bool rayIntersects(Ray & ray, float& t, float& u, float& v) = 0;
};

