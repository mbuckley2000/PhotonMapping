#include "pch.h"
#include "Scene.h"
#include <Eigen/Dense>

using namespace Eigen;

void Scene::render(cv::Mat_<cv::Vec3b>& target)
{
	for (int x = 0; x < camera->imagePlane.resolution(0); x++) {
		for (int y = 0; y < camera->imagePlane.resolution(1); y++) {
			const Vector3d pixelWorldPos = this->camera->calculatePixelWorldPos(x, y);

			Ray ray(pixelWorldPos, pixelWorldPos - this->camera->focus); //Pos, dir

			for (auto const& instance : this->objectInstances) {
				if (ray.intersectsWith(*instance)) {
					target(x, y) = instance->colour;
				}
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
