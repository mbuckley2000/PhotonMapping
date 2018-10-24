#pragma once
#include <Eigen\Dense>
#include "Object.h"
#include "Ray.h"

class Ray;
class Object;

class ObjectInstance
{
public:
	Eigen::Vector3d position;
	Object *object;
	cv::Vec3b colour;

	bool rayIntersects(Ray& ray);

	ObjectInstance(Object& object);
	virtual ~ObjectInstance();
};

