#pragma once
#include "Object.h"
#include <glm/vec3.hpp>

class Triangle :
	public Object
{
public:
	bool rayIntersects(Ray& ray, float& t, float& u, float& v);

	Triangle(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2);

	glm::vec3 normal;
	glm::vec3 point;
	glm::vec3 edge1;
	glm::vec3 edge2;
	glm::vec3 vertex[3];

	virtual ~Triangle();
private:
	void calculatePlane();
};