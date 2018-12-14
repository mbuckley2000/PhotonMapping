#pragma once
#include "Vectors.h"
#include "Camera.h"
#include "Scene.h"

class ProjectionMap
{
public:
	ProjectionMap(Vec3 position, Vec3 direction);
	~ProjectionMap();

	void updateMap(Scene& scene);
	bool directionIsInBounds(Vec3 direction);

private:
	Camera camera;
	bool** map;

	bool traceRay(Ray& ray, Scene& scene);
};

