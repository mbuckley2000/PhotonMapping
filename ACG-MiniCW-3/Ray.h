#pragma once
#include <Eigen/Dense>
#include "Object.h"
#include "Scene.h"


class Scene;
class Object;

class Ray
{
public:
	Eigen::Vector3f position;
	Eigen::Vector3f direction;

	bool intersectsWith(Object& object, float& t, float& u, float& v);
	bool intersectsWith(Scene & scene);
	bool intersectsWith(Scene & scene, Object* closestObject, float & t, float & u, float & v);

	Ray(Eigen::Vector3f position, Eigen::Vector3f direction);
	virtual ~Ray();
};

