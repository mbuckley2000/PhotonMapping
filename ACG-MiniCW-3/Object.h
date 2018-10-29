#pragma once
#include "Ray.h"
#include <Eigen/Dense>

class Ray;
class Sphere;

class Object
{
public:
	Eigen::Vector3f position;
	cv::Vec3b colour;
	Sphere* boundingSphere;

	Object() {}
	virtual ~Object() {}

	virtual bool rayIntersects(Ray & ray, float& t, float& u, float& v) = 0;
};

