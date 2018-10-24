#include "pch.h"
#include "Triangle.h"

using namespace Eigen;


//From https://en.wikipedia.org/wiki/M%C3%B6ller%E2%80%93Trumbore_intersection_algorithm
bool Triangle::rayIntersects(Ray & ray, Eigen::Vector3d position)
{
	const double EPSILON = 0.0000001;
	const Vector3d rayOrigin = ray.position;
	const Vector3d rayVector = ray.direction;
	const Vector3d vertex0 = this->vertices.row(0).transpose() + position;
	const Vector3d vertex1 = this->vertices.row(1).transpose() + position;
	const Vector3d vertex2 = this->vertices.row(2).transpose() + position;

	const Vector3d edge1 = vertex1 - vertex0;
	const Vector3d edge2 = vertex2 - vertex0;

	const Vector3d h = rayVector.cross(edge2);
	const double a = edge1.dot(h);

	if (a > -EPSILON && a < EPSILON)
		return false;    // This ray is parallel to this triangle.
	const double f = 1.0 / a;
	const Vector3d s = rayOrigin - vertex0;
	const double u = f * (s.dot(h));
	if (u < 0.0 || u > 1.0)
		return false;
	const Vector3d q = s.cross(edge1);
	const double v = f * rayVector.dot(q);
	if (v < 0.0 || u + v > 1.0)
		return false;
	// At this stage we can compute t to find out where the intersection point is on the line.
	const double t = f * edge2.dot(q);
	if (t > EPSILON) // ray intersection
	{
		const Vector3d intersectionPoint = rayOrigin + rayVector * t;
		//Could return this
		return true;
	}
	else // This means that there is a line intersection but not a ray intersection.
		return false;
}

Triangle::Triangle(Eigen::Matrix3d vertices)
{
	this->vertices = vertices;
}


Triangle::~Triangle()
{
}
