#include "pch.h"
#include "Triangle.h"

//https://en.wikipedia.org/wiki/M%C3%B6ller%E2%80%93Trumbore_intersection_algorithm
bool Triangle::rayIntersects(Ray & ray, Object*& o, float& t, float& u, float& v)
{
	#define EPSILON 0.0000001
	#define CULLING true

	const Vec3 pVec = ray.direction.cross(this->edge2);

	const float det = edge1.dot(pVec);

	if (CULLING) {
		// Negative det means the triangle isn't facing the camera
		// Det near 0 is a miss
		if (det < EPSILON) {
			return false;
		}
	}
	else {
		// They are parallel
		if (fabs(det) < EPSILON) {
			return false;
		}
	}

	float detInv = 1 / det;

	const Vec3 tVec = ray.position - this->vertices.col(0);

	u = tVec.dot(pVec) * detInv;

	if (u < 0 || u > 1) {
		return false;
	}

	const Vec3 qvec = tVec.cross(this->edge1);

	v = ray.direction.dot(qvec) * detInv;

	if (v < 0 || u + v > 1) {
		return false;
	}

	t = this->edge2.dot(qvec) * detInv;

	o = this;

	return true;
}

Vec3 Triangle::getNormalAt(Vec3 position)
{
	return this->normal;
}

Vec3 Triangle::getVertex(int vertex)
{
	return Vec3(this->vertices.col(vertex));
}

Triangle::Triangle(Vec3 v0, Vec3 v1, Vec3 v2)
{
	this->vertices.col(0) = v0;
	this->vertices.col(1) = v1;
	this->vertices.col(2) = v2;

	//Pre-calculate edges for efficiency
	this->edge1 = vertices.col(1) - vertices.col(0);
	this->edge2 = vertices.col(2) - vertices.col(0);

	//Pre-calc face normal
	this->normal = edge1.cross(edge2).normalized();
}

Triangle::Triangle(Mat3 vertices)
{
	this->vertices = vertices;

	//Pre-calculate edges for efficiency
	this->edge1 = vertices.col(1) - vertices.col(0);
	this->edge2 = vertices.col(2) - vertices.col(0);

	//Pre-calc face normal
	this->normal = edge1.cross(edge2).normalized();
}

Triangle::~Triangle()
{
}

