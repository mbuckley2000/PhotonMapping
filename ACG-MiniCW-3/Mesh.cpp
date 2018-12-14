#include "pch.h"
#include "Mesh.h"
#include <igl/readOBJ.h>
#include "Box.h"

using namespace Eigen;

void Mesh::loadFromFile(std::string filename)
{
	//igl::readOBJ(filename, this->vertices, this->faces);

	Eigen::Matrix2Xf texCoords;
	Eigen::MatrixXi faceTex; //   FTC  #F list of face indices into vertex texture coordinates
	
	igl::readOBJ(filename, this->vertices, texCoords, this->vertexNormals, this->faces, faceTex, this->faceVns);
}


bool Mesh::rayIntersects(Ray & ray, Object*& o, float& t)
{
	return this->kdTree->intersect(ray, o, t);
}

void Mesh::calculateTriangles()
{
	this->triangles.empty();

	for (int f = 0; f < this->faces.rows(); f++) {
		const Vector3i face = this->faces.row(f);
		const Vector3i faceVns = this->faceVns.row(f);

		Matrix3f vs;
		Matrix3f vns;

		for (int v = 0; v < 3; v++) {
			vs.col(v) = (this->vertices.row(face(v)).transpose() * this->scale) + this->position;
			vns.col(v) = (this->vertexNormals.row(faceVns(v)).transpose());
		}

		Triangle* t = new Triangle(vs, vns);
		t->material = this->material;

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

Vec3 Mesh::getNormalAt(Vec3 position)
{
	return Vec3();
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

void Mesh::buildKDTree()
{
	this->kdTree = new TriangleKDNode(this->triangles);
}
