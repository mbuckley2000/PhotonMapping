#include "pch.h"
#include "Triangle.h"

using namespace Eigen;


const double kEpsilon = 0.0000001;
const bool culling = true;

bool rayTriangleIntersect(
	const Vector3f &orig,
	const Vector3f &dir,
	const Vector3f &v0,
	const Vector3f &v1,
	const Vector3f &v2,
	float &t,
	float &u,
	float &v
) {

	Vector3f v0v1 = v1 - v0;
	Vector3f v0v2 = v2 - v0;
	Vector3f pvec = dir.cross(v0v2);
	float det = v0v1.dot(pvec);
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

	Vector3f tvec = orig - v0;
	u = tvec.dot(pvec) * invDet;
	if (u < 0 || u > 1) return false;

	Vector3f qvec = tvec.cross(v0v1);
	v = dir.dot(qvec) * invDet;
	if (v < 0 || u + v > 1) return false;

	t = v0v2.dot(qvec) * invDet;

	return true;
}



//From https://en.wikipedia.org/wiki/M%C3%B6ller%E2%80%93Trumbore_intersection_algorithm
bool Triangle::rayIntersects(Ray & ray)
{
	float t, u, v;
	return rayTriangleIntersect(ray.position, ray.direction, this->vertices.col(0), this->vertices.col(1), this->vertices.col(2), t, u, v);
	/*
	const double EPSILON = 0.0000001;
	const Vector3f rayOrigin = ray.position;
	const Vector3f rayVector = ray.direction;
	const Vector3f vertex0 = this->vertices.col(0);
	const Vector3f vertex1 = this->vertices.col(1);
	const Vector3f vertex2 = this->vertices.col(2);

	const Vector3f edge1 = vertex1 - vertex0;
	const Vector3f edge2 = vertex2 - vertex0;

	const Vector3f h = rayVector.cross(edge2);
	const float a = edge1.dot(h);

	if (a > -EPSILON && a < EPSILON)
		return false;    // This ray is parallel to this triangle.
	const double f = 1.0 / a;
	const Vector3f s = rayOrigin - vertex0;
	const float u = f * (s.dot(h));
	if (u < 0.0 || u > 1.0)
		return false;
	const Vector3f q = s.cross(edge1);
	const float v = f * rayVector.dot(q);
	if (v < 0.0 || u + v > 1.0)
		return false;
	// At this stage we can compute t to find out where the intersection point is on the line.
	const float t = f * edge2.dot(q);
	if (t > EPSILON) // ray intersection
	{
		const Vector3f intersectionPoint = rayOrigin + rayVector * t;
		//Could return this
		return true;
	}
	else // This means that there is a line intersection but not a ray intersection.
		return false;
		*/
}

Triangle::Triangle(Eigen::Matrix3f vertices)
{
	this->vertices = vertices;
}


Triangle::~Triangle()
{
}
