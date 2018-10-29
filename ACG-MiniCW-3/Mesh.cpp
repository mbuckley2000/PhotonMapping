#include "pch.h"
#include "Mesh.h"
#include <Eigen/Dense>
#include <igl/readOBJ.h>
#include <glm/vec3.hpp>

void Mesh::loadFromFile(std::string filename)
{
	std::cout << "Loading mesh from file" << std::endl;
	igl::readOBJ(filename, this->vertices, this->faces);
}

bool Mesh::rayIntersects(Ray & ray, float& t, float& u, float& v)
{
	if (!this->trianglesCalculated) {
		this->calculateTriangles();
	}

	/*if (!this->boundingSphereCalculated) {
		this->calculateBoundingSphere();
	}*/

	float smallestT = INFINITY;

	for (auto const& triangle : this->triangles) {
		if (triangle->rayIntersects(ray, t, u, v)) {

			if (t < smallestT) {
				smallestT = t;
			}
		}
	}

	if (smallestT != INFINITY) return true;
	
	return false;
}

void Mesh::calculateTriangles()
{
	std::cout << "Calculating mesh triangles" << std::endl;

	this->triangles.empty();

	for (int f = 0; f < this->faces.rows(); f++) {
		const Eigen::Vector3i face = this->faces.row(f);

		glm::vec3 vertex[3];

		for (int v = 0; v < 3; v++) {
			Eigen::Vector3f vert = this->vertices.row(face(v)).transpose();
			vertex[v][0] = vert[0];
			vertex[v][1] = vert[1];
			vertex[v][2] = vert[2];
			vertex[v] += this->position;
		}

		//Convert from Eigen to glm

		this->triangles.push_back(new Triangle(vertex[0], vertex[1], vertex[2]));
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
	this->calculateTriangles();
}


Mesh::~Mesh()
{
}
