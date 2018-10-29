#include "pch.h"
#include "Scene.h"
#include "Ray.h"
#include <glm/vec3.hpp>

void Scene::render()
{
	std::cout << "Rendering" << std::endl;

	for (int x = 0; x < camera->imagePlane.resolution[0]; x++) {
		for (int y = 0; y < camera->imagePlane.resolution[1]; y++) {

			const glm::vec3 pixelWorldPos = this->camera->calculatePixelWorldPos(x, y);

			Ray ray(pixelWorldPos, pixelWorldPos - this->camera->focus); //Pos, dir

			Object* closestObject;
			float smallestT, t, u, v;
			smallestT = INFINITY;

			for (auto const& object : this->objects) {
				if (ray.intersectsWith(*object, t, u, v)) {
					if (t < smallestT) {
						t = smallestT;
						closestObject = object;
					}
				}
			}

			if (smallestT != INFINITY) {
				std::cout << "HIT" << std::endl;
				(*this->target)(x, y) = (0, 0, 1);//closestObject->colour;
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
