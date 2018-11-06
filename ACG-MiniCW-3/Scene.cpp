#include "pch.h"
#include "Scene.h"
#include <Eigen/Dense>
#include "Ray.h"

using namespace Eigen;

float max(float a, float b) {
	if (a > b) {
		return a;
	}
	else {
		return b;
	}
}

float min(float a, float b) {
	if (a < b) {
		return a;
	}
	else {
		return b;
	}
}

cv::Vec3b toRGB(Eigen::Vector3f v) {
	for (int i = 0; i < 3; i++) {
		v(i) = max(v(i), 0);
		v(i) = min(v(i), 1);
	}

	return cv::Vec3b(v(0) * 255, v(1) * 255, v(2) * 255);
}

Eigen::Vector3f reflect(Eigen::Vector3f vec, Eigen::Vector3f normal) {
	return vec - (2 * vec.dot(normal) * normal);
}


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

				//Ambient
				const Eigen::Vector3f ambient = closestObj->ambient * light->colour;

				//Diffuse
				const Eigen::Vector3f intersectionPoint = ray.position + (closestT * ray.direction);
				const Eigen::Vector3f lightVector = -light->vectorTo(intersectionPoint);
				const Eigen::Vector3f faceNormal = closestObj->getNormalAt(intersectionPoint);

				Eigen::Vector3f diffuse;
				diffuse << 0, 0, 0;

				const float dot = max(faceNormal.dot(lightVector), 0);

				if (dot >= 0) {
					diffuse = dot * light->colour;
				}

				//Specular
				const Eigen::Vector3f viewVector = (pixelWorldPos - intersectionPoint).normalized();
				const Eigen::Vector3f reflectionVector = reflect(-lightVector, faceNormal);
				float spec = pow(max(viewVector.dot(reflectionVector), 0), 32);
				const Eigen::Vector3f specular = closestObj->specular * spec * light->colour;

				const Eigen::Vector3f colour = (ambient + diffuse + specular).cwiseProduct(closestObj->colour);

				(*this->target)(x, y) = toRGB(colour);
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
