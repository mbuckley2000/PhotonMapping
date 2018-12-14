#include "pch.h"
#include "Triangle.h"


void Triangle::getBarycentricCoordinates(Vec3 p, float &u, float &v, float &w)
{
	//Copied from https://gamedev.stackexchange.com/questions/23743/whats-the-most-efficient-way-to-find-barycentric-coordinates
	const Vec3 a = this->getVertex(0);
	const Vec3 b = this->getVertex(1);
	const Vec3 c = this->getVertex(2);
	Vec3 v0 = b - a, v1 = c - a, v2 = p - a;
	float d00 = v0.dot(v0);
	float d01 = v0.dot(v1);
	float d11 = v1.dot(v1);
	float d20 = v2.dot(v0);
	float d21 = v2.dot(v1);
	float denom = d00 * d11 - d01 * d01;
	v = (d11 * d20 - d01 * d21) / denom;
	w = (d00 * d21 - d01 * d20) / denom;
	u = 1.0f - v - w;
}

bool Triangle::rayIntersects(Ray & ray, Object*& o, float& t)
{

	/**
	 * Möller–Trumbore intersection algorithm
	 * https://en.wikipedia.org/wiki/M%C3%B6ller%E2%80%93Trumbore_intersection_algorithm
	 */

	#define EPSILON 0.0000001
	#define CULLING false

	float u, v;

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

	const float detInv = 1 / det;

	const Vec3 tVec = ray.position - this->getVertex(0);

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
	if (this->vertexNormals.size() != 3) {
		return this->normal;
	}

	float u, v, w;
	this->getBarycentricCoordinates(position, u, v, w);

	return (this->vertexNormals[0] * u + this->vertexNormals[1] * v + this->vertexNormals[2] * w).normalized();
}

Vec3 Triangle::getVertex(int vertex)
{
	return this->vertices.col(vertex);
}

Box Triangle::getBoundingBox()
{
	if (!this->boundingBoxCalculated) {
		Box b;
		b.minimum << INFINITY, INFINITY, INFINITY;
		b.maximum << -INFINITY, -INFINITY, -INFINITY;

		for (int i = 0; i < 3; i++) {
			b.expand(this->getVertex(i));
		}

		this->boundingBoxCalculated = true;
		this->boundingBox = b;
		return b;
	}

	return this->boundingBox;
}

Vec3 Triangle::getMidpoint()
{
	if (!this->midPointCalculated) {
		Vec3 midPoint(0,0,0);

		for (int i = 0; i < 3; i++) {
			midPoint += this->getVertex(i);
		}

		midPoint /= 3;

		this->midPointCalculated = true;
		this->midPoint = midPoint;
		return midPoint;
	}

	return this->midPoint;
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

Triangle::Triangle(Mat3 vertices, Mat3 vertexNormals)
{
	this->vertices = vertices;

	//Pre-calculate edges for efficiency
	this->edge1 = vertices.col(1) - vertices.col(0);
	this->edge2 = vertices.col(2) - vertices.col(0);

	//Pre-calc face normal
	this->normal = edge1.cross(edge2).normalized();

	//Push vns
	for (int i = 0; i < 3; i++) {
		this->vertexNormals.push_back(vertexNormals.col(i));
	}
}