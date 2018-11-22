#include "pch.h"
#include "Mesh.h"
#include <igl/readOBJ.h>
#include "Box.h"

using namespace Eigen;

void Mesh::loadFromFile(std::string filename)
{
	igl::readOBJ(filename, this->vertices, this->faces);
}

bool Mesh::rayIntersects(Ray & ray, Object*& o, float& t, float& u, float& v)
{
	if (!this->trianglesCalculated) {
		this->calculateTriangles();
	}

	if (!this->boundingBoxCalculated) {
		this->calculateBoundingBox();
	}

	float tt, uu, vv;
	Object* oo = NULL;
	if (!this->boundingBox.rayIntersects(ray, oo, tt, uu, vv)) {
		return false;
	}

	float currentT, currentU, currentV;
	float closestT = INFINITY, closestU, closestV;
	Triangle* closestTriangle = NULL;

	Object* p = NULL;

	for (auto const& triangle : this->triangles) {
		if (triangle->rayIntersects(ray, p, currentT, currentU, currentV)) {
			if (currentT < closestT) {
				closestT = currentT;
				closestU = currentU;
				closestV = currentV;
				closestTriangle = triangle;
			}
		}
	}
	
	if (closestTriangle != NULL) {
		t = closestT;
		u = closestU;
		v = closestV;
		o = closestTriangle;
		return true;
	}

	return false;
}

Vec3 Mesh::getNormalAt(Vec3 position)
{
	return Vec3();
}

void Mesh::calculateTriangles()
{
	this->triangles.empty();

	for (int f = 0; f < this->faces.rows(); f++) {
		const Vector3i face = this->faces.row(f);

		Matrix3f vs;

		for (int v = 0; v < 3; v++) {
			vs.col(v) = this->vertices.row(face(v)).transpose() + this->position;
		}

		Triangle* t = new Triangle(vs);
		t->colour = this->colour;
		t->ambient = this->ambient;
		t->specularCoeff = this->specularCoeff;
		t->specularPower = this->specularPower;

		this->triangles.push_back(t);
	}
	
	this->trianglesCalculated = true;
}

Mesh::Mesh(std::string filename)
{
	loadFromFile(filename);
	trianglesCalculated = false;
	boundingBoxCalculated = false;
}


Mesh::~Mesh()
{
}

void Mesh::calculateBoundingBox()
{
	Box* b = &this->boundingBox;

	b->maximum = Vec3(-INFINITY, -INFINITY, -INFINITY);
	b->minimum = Vec3(INFINITY, INFINITY, INFINITY);

	for (auto const& triangle : this->triangles) {
		for (int v = 0; v < 3; v++) {
			const Vec3 vertex = triangle->getVertex(v);

			for (int d = 0; d < 3; d++) {
				if (vertex(d) < b->minimum(d)) {
					b->minimum(d) = vertex(d);
				}

				if (vertex(d) > b->maximum(d)) {
					b->maximum(d) = vertex(d);
				}
			}
		}
	}
	this->boundingBoxCalculated = true;
}
