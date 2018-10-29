#pragma once
#include <glm/vec3.hpp>
#include "Object.h"

class Object;

class Ray
{
public:
	glm::vec3 position;
	glm::vec3 direction;

	bool intersectsWith(Object& object, float& t, float& u, float& v);

	Ray(glm::vec3 position, glm::vec3 direction);
	virtual ~Ray();
};

