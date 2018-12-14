#include "pch.h"
#include "ProjectionMap.h"


ProjectionMap::ProjectionMap(Vec3 position, Vec3 direction)
{
	const int xRes = 1000;
	const int yRes = 1000;

	this->camera = Camera();
	this->camera.focus = position;
	this->camera.backward = -direction.normalized();
	this->camera.right << 1, 0, 0;
	this->camera.up << 0, 1, 0;
	this->camera.focalLength = 0.1;
	this->camera.imagePlane.resolution << xRes, yRes;
	this->camera.imagePlane.worldSize << 1, 1;

	//Allocate map memory
	this->map = (bool**)malloc(sizeof(bool*) * yRes);
	for (int y = 0; y < yRes; y++) {
		this->map[y] = (bool*) malloc(sizeof(bool) * xRes);
	}
}

ProjectionMap::~ProjectionMap()
{
	for (int y = 0; y < this->camera.imagePlane.resolution(1); y++) {
		free(this->map[y]);
	}

	free(this->map);
}

void ProjectionMap::updateMap(Scene & scene)
{
	//Just like rendering

	for (int x = 0; x < this->camera.imagePlane.resolution(0); x++) {
		for (int y = 0; y < this->camera.imagePlane.resolution(1); y++) {

			const Vec3 pixelWorldPos = this->camera.calculatePixelWorldPos(x, y);

			Ray ray(pixelWorldPos, (pixelWorldPos - this->camera.focus).normalized()); //Pos, dir

			this->map[y][x] = this->traceRay(ray, scene);
		}
	}
}

bool ProjectionMap::directionIsInBounds(Vec3 direction)
{
	return false;
}

bool ProjectionMap::traceRay(Ray & ray, Scene & scene)
{
	float t;
	Object* hitObj;

	if (ray.intersectsWith(scene, hitObj, t)) {
		return hitObj->material.refractive;
	}

	return false;
}
