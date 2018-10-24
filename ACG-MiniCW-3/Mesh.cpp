#include "pch.h"
#include "Mesh.h"
#include <igl/readOBJ.h>

using namespace Eigen;

void Mesh::loadFromFile(std::string filename)
{
	igl::readOBJ(filename, this->vertices, this->faces);
	this->calculateTriangles();
}

bool Mesh::rayIntersects(Ray & ray, Eigen::Vector3d position)
{
	if (!this->trianglesCalculated) {
		this->calculateTriangles();
	}

	for (auto const& triangle : this->triangles) {
		if (triangle->rayIntersects(ray, position)) {
			return true;
		}
	}
	
	return false;
}

void Mesh::calculateTriangles()
{
	for (int f = 0; f < this->faces.rows(); f++) {
		const Vector3i face = this->faces.row(f);

		Matrix3d vs;

		for (int v = 0; v < 3; v++) {
			
			vs.row(v) = this->vertices.row(face(v));
		}

		this->triangles.push_back(new Triangle(vs));
	}
	
}

Mesh::Mesh()
{
	trianglesCalculated = false;
}


Mesh::~Mesh()
{
}
