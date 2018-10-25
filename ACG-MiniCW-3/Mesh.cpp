#include "pch.h"
#include "Mesh.h"
#include <igl/readOBJ.h>

using namespace Eigen;

void Mesh::loadFromFile(std::string filename)
{
	igl::readOBJ(filename, this->vertices, this->faces);
}

bool Mesh::rayIntersects(Ray & ray)
{
	if (!this->trianglesCalculated) {
		this->calculateTriangles();
	}

	if (!this->boundingSphereCalculated) {
		this->calculateBoundingSphere();
	}

	for (auto const& triangle : this->triangles) {
		if (triangle->rayIntersects(ray)) {
			return true;
		}
	}
	
	return false;
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
