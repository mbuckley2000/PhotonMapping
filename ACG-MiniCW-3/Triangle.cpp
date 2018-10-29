#include "pch.h"
#include "Triangle.h"
#include <glm/vec3.hpp>
#include <glm/geometric.hpp>

#define CULLING true
#define EPSILON 0.0000001

//From https://en.wikipedia.org/wiki/M%C3%B6ller%E2%80%93Trumbore_intersection_algorithm
bool Triangle::rayIntersects(Ray & ray, float& t, float& u, float& v)
{
	const glm::vec3 orig = ray.position;
	const glm::vec3 dir = ray.direction;
	const glm::vec3 v0 = this->vertex[0];
	const glm::vec3 v1 = this->vertex[0];
	const glm::vec3 v2 = this->vertex[0];
	
	const glm::vec3 edge1 = this->edge1;
	const glm::vec3 edge2 = this->edge2;

	const glm::vec3 pvec = glm::cross(dir, edge2);
	const float det = glm::dot(edge1, pvec);

	if (CULLING) {
		// if the determinant is negative the triangle is backfacing
		// if the determinant is close to 0, the ray misses the triangle
		if (det < EPSILON) return false;
	}
	else {
		// ray and triangle are parallel if det is close to 0
		if (fabs(det) < EPSILON) return false;
	}

	float invDet = 1 / det;

	const glm::vec3 tvec = orig - v0;

	u =  glm::dot(tvec, pvec) * invDet;
	if (u < 0 || u > 1) return false;

	const glm::vec3 qvec = glm::cross(tvec, edge1);

	v = glm::dot(dir, qvec) * invDet;
	if (v < 0 || u + v > 1) return false;

	t = glm::dot(edge2, qvec) * invDet;

	return true;
}



Triangle::Triangle(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2)
{
	this->vertex[0] = v0;
	this->vertex[1] = v1;
	this->vertex[2] = v2;
	this->calculatePlane();
}


Triangle::~Triangle()
{
}

void Triangle::calculatePlane()
{
	//Calculate edges
	this->edge1 = this->vertex[1] - this->vertex[0];
	this->edge2 = this->vertex[2] - this->vertex[0];

	//Calculate normal
	this->normal = glm::cross(this->edge1, this->edge2);

	//Point is one of the vertices
	this->point = vertex[0];
}
