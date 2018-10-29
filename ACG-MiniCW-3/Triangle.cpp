#include "pch.h"
#include "Triangle.h"

using namespace Eigen;



const double kEpsilon = 0.0000001;
const bool culling = true;

//From https://en.wikipedia.org/wiki/M%C3%B6ller%E2%80%93Trumbore_intersection_algorithm
bool Triangle::rayIntersects(Ray & ray, float& t, float& u, float& v)
{
	const Vector3f orig = ray.position;
	const Vector3f dir = ray.direction;
	const Vector3f v0 = this->vertices.col(0);
	const Vector3f v1 = this->vertices.col(1);
	const Vector3f v2 = this->vertices.col(2);

	const Vector3f edge1 = this->edge1;
	const Vector3f edge2 = this->edge2;

	const Vector3f pvec = dir.cross(edge2);
	const float det = edge1.dot(pvec);

	if (culling) {
		// if the determinant is negative the triangle is backfacing
		// if the determinant is close to 0, the ray misses the triangle
		if (det < kEpsilon) return false;
	}
	else {
		// ray and triangle are parallel if det is close to 0
		if (fabs(det) < kEpsilon) return false;
	}

	float invDet = 1 / det;

	const Vector3f tvec = orig - v0;

	u = tvec.dot(pvec) * invDet;
	if (u < 0 || u > 1) return false;

	const Vector3f qvec = tvec.cross(edge1);

	v = dir.dot(qvec) * invDet;
	if (v < 0 || u + v > 1) return false;

	t = edge2.dot(qvec) * invDet;

	return true;
}

Triangle::Triangle(Eigen::Matrix3f vertices)
{
	this->vertices = vertices;
	this->edge1 = vertices.col(1) - vertices.col(0);
	this->edge2 = vertices.col(2) - vertices.col(0);

}


Triangle::~Triangle()
{
}


/*
#include "pch.h"
#include "Triangle.h"
#include <glm/vec3.hpp>
#include <glm/geometric.hpp>

#define CULLING true
#define EPSILON 0.0000001

//From https://en.wikipedia.org/wiki/M%C3%B6ller%E2%80%93Trumbore_intersection_algorithm
bool Triangle::rayIntersects(Ray & ray, float& t, float& u, float& v)
{
	const Vector3f orig = ray.position;
	const Vector3f dir = ray.direction;
	const Vector3f v0 = this->vertex[0];

	const Vector3f edge1 = this->edge1;
	const Vector3f edge2 = this->edge2;

	const Vector3f pvec = glm::cross(dir, edge2);
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

	const Vector3f tvec = orig - v0;

	u =  glm::dot(tvec, pvec) * invDet;
	if (u < 0 || u > 1) return false;

	const Vector3f qvec = glm::cross(tvec, edge1);

	v = glm::dot(dir, qvec) * invDet;
	if (v < 0 || u + v > 1) return false;

	t = glm::dot(edge2, qvec) * invDet;

	return true;
}



Triangle::Triangle(Vector3f v0, Vector3f v1, Vector3f v2)
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
*/