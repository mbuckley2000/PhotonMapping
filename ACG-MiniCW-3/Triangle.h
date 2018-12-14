#pragma once
#include "Object.h"
#include "Vectors.h"
#include "Box.h"

class Triangle :
	public Object
{
public:
	/**
	 * Construct from three vertices
	 */
	Triangle(Vec3 v0, Vec3 v1, Vec3 v2);

	/**
	 * Construct from a 3x3 matrix containing the vertices and another containing the normals.
	 */
	Triangle(Mat3 vertices, Mat3 vertexNormals);

	bool rayIntersects(Ray & ray, Object*& o, float& t);

	/**
	 * If vertex normals have been loaded, the normal will be interpolated using Phong Interpolation
	 * If not, the normal will be constant and perpendicular to the edges of the triangle
	 */
	Vec3 getNormalAt(Vec3 position);

	/**
	 * Returns the vertex (0, 1, 2) as a vector
	 */
	Vec3 getVertex(int vertex);

	/**
	 * Calculated a bounding box containing all vertices of the triangle
	 * WIll only be calculated once and then cached
	 */
	Box getBoundingBox();

	/**
	 * Calculates the mid (average) point of the triangle
	 * Will only be calculated once and then cached
	 */
	Vec3 getMidpoint();

private:
	Vec3 edge1;
	Vec3 edge2;
	Vec3 normal;
	Box boundingBox;
	bool boundingBoxCalculated = false;
	Vec3 midPoint;
	bool midPointCalculated = false;
	Mat3 vertices;
	std::vector<Vec3> vertexNormals;

	/**
	 * Copied from https://gamedev.stackexchange.com/questions/23743/whats-the-most-efficient-way-to-find-barycentric-coordinates
	 * Compute barycentric coordinates (u, v, w) of a point p on the triangle
	 */
	void getBarycentricCoordinates(Vec3 p, float &u, float &v, float &w);
};

