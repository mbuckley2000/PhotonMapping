#include "pch.h"
#include "Triangle.h"

using namespace Eigen;

const double EPSILON = 0.0000001;
const bool CULLING = true;

//https://en.wikipedia.org/wiki/M%C3%B6ller%E2%80%93Trumbore_intersection_algorithm
bool Triangle::rayIntersects(Ray & ray, float& t, float& u, float& v)
{
	//Triangle ray intersection
	const Vector3f rayPos = ray.position;
	const Vector3f rayDir = ray.direction;
	const Vector3f v0 = this->vertices.col(0);
	const Vector3f edge1 = this->edge1;
	const Vector3f edge2 = this->edge2;

	const Vector3f pVec = rayDir.cross(edge2);

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

	const Vector3f tVec = rayPos - v0;

	u = tVec.dot(pVec) * detInv;

	if (u < 0 || u > 1) {
		return false;
	}

	const Vector3f qvec = tVec.cross(edge1);

	v = rayDir.dot(qvec) * detInv;

	if (v < 0 || u + v > 1) {
		return false;
	}

	t = edge2.dot(qvec) * detInv;

	return true;
}

Eigen::Vector3f Triangle::getNormalAt(Eigen::Vector3f position)
{
	return this->normal;
}

Triangle::Triangle(Eigen::Matrix3f vertices)
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