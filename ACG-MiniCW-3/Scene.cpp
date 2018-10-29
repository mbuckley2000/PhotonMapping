#include "pch.h"
#include "Scene.h"
#include <Eigen/Dense>
#include "Ray.h"

using namespace Eigen;

void Scene::render()
{

	float t, u, v;

	for (int x = 0; x < camera->imagePlane.resolution(0); x++) {
		for (int y = 0; y < camera->imagePlane.resolution(1); y++) {

			const Vector3f pixelWorldPos = this->camera->calculatePixelWorldPos(x, y);

			Ray ray(pixelWorldPos, pixelWorldPos - this->camera->focus); //Pos, dir

			float closestT = INFINITY;
			Object* closestObj = NULL;

			for (auto const& object : this->objects) {
				if (ray.intersectsWith(*object, t, u, v)) {
					if (t < closestT) {
						closestT = t;
						closestObj = object;
					}
				}
			}

			if (closestObj != NULL) {
				(*this->target)(x, y) = closestObj->colour;
			}
		}
	}
}

Scene::Scene()
{
}


Scene::~Scene()
{
}
