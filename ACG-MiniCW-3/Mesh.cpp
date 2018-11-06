#include "pch.h"
#include "Mesh.h"
#include <igl/readOBJ.h>

using namespace Eigen;

void Mesh::loadFromFile(std::string filename)
{
	igl::readOBJ(filename, this->vertices, this->faces);
}

bool Mesh::rayIntersects(Ray & ray, float& t, float& u, float& v)
{
	if (!this->trianglesCalculated) {
		this->calculateTriangles();
	}

	if (!this->boundingSphereCalculated) {
		this->calculateBoundingSphere();
	}

	float currentT, currentU, currentV;
	float closestT = INFINITY, closestU, closestV;
	Triangle* closestTriangle = NULL;

	for (auto const& triangle : this->triangles) {
		if (triangle->rayIntersects(ray, currentT, currentU, currentV)) {
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
		return true;
	}

	return false;
}

Eigen::Vector3f Mesh::getNormalAt(Eigen::Vector3f position)
{
	return Eigen::Vector3f();
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

		this->triangles.push_back(new Triangle(vs));
	}
	
	this->trianglesCalculated = true;
}

void Mesh::calculateBoundingSphere()
{

}

Mesh::Mesh(std::string filename)
{
	loadFromFile(filename);
	trianglesCalculated = false;
}


Mesh::~Mesh()
{
}
