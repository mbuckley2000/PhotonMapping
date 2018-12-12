#include "pch.h"
#include "Scene.h"
#include "Ray.h"
#include "Vectors.h"
#include "Common.h"
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include "BRDF.h"
#include "Triangle.h"
#include <thread>

# define M_PI           3.14159265358979323846  /* pi */
# define EPSILON 0.00001

cv::Vec3b toRGB(Vec3 v) {
	for (int i = 0; i < 3; i++) {
		v(i) = max(v(i), 0);
		v(i) = min(v(i), 1);
	}


	return cv::Vec3b(v(2) * 255, v(1) * 255, v(0) * 255);
}

void Scene::tracerThread(int threadID, int numThreads) {
	for (int x = threadID; x < camera->imagePlane.resolution(0); x+=numThreads) {
		for (int y = 0; y < camera->imagePlane.resolution(1); y++) {

			const Vec3 pixelWorldPos = this->camera->calculatePixelWorldPos(x, y);

			Ray ray(pixelWorldPos, pixelWorldPos - this->camera->focus); //Pos, dir

			const float dimmingFactor = 5;
			(*this->target)(y, x) = toRGB(this->traceRay(&ray, 0, 4) * dimmingFactor);
		}
		this->lineDone[x] = true;
	}
}

//Returns colour
Vec3 Scene::traceRay(Ray* ray, int depth, int maxDepth) {
	const bool SHADOWING = false;
	const bool DIRECTILLUMINATION = true;
	const bool PMAP = true;

	Vec3 colour = Vec3(0, 0, 0);

	float t, u, v;
	Object* hitObj = NULL;

	if (ray->intersectsWith(*this, hitObj, t, u, v)) {

		//We have a hit
		const Vec3 normalisedRayDir = ray->direction.normalized();
		const Vec3 intersectionPoint = ray->position + (t * normalisedRayDir);
		Material* material = &(hitObj->material);
		Vec3 hitNormal = hitObj->getNormalAt(intersectionPoint);

		//Reflections & refractions
		if (material->refractive && depth < maxDepth) {
			Vec3 refractiveComponent(0,0,0);
			Vec3 reflectiveComponent(0,0,0);

			if (material->reflectiveness > 0) {
				Ray reflectiveRay = Ray(intersectionPoint, reflectVector(-normalisedRayDir, hitNormal));
				reflectiveRay.position += EPSILON * reflectiveRay.direction;
				reflectiveComponent = material->reflectiveness * this->traceRay(&reflectiveRay, depth + 1, maxDepth);
			}

			if (material->refractiveness > 0) {
				Ray refractiveRay = Ray(intersectionPoint, refractVector(normalisedRayDir, hitNormal, material->indexOfRefraction));
				refractiveRay.position += EPSILON * refractiveRay.direction;
				refractiveComponent = material->refractiveness * this->traceRay(&refractiveRay, depth + 1, maxDepth);
			}

			return (refractiveComponent + reflectiveComponent);
		}

		bool shadowed = false;
		int num_rays = 16;
		int hitCount = 0;
		if (SHADOWING) {
			//Check for shadow
			if (hitObj->shadow) {
				for (int i = 0; i < num_rays; i++) {
					Ray shadowTest = Ray(intersectionPoint, -light->vectorTo(intersectionPoint));
					shadowTest.position = shadowTest.position + (EPSILON * shadowTest.direction);
					float shadowT;
					if (shadowTest.intersectsWith(*this, shadowT)) {
						if (shadowT <= (light->getDistanceFrom(intersectionPoint))) {
							hitCount++;
							shadowed = true;
						}
					}
				}
			}
		}

		//BRDF
		const Vec3 lightVector = -light->vectorTo(intersectionPoint).normalized();
		const Vec3 viewVector = (ray->position - intersectionPoint).normalized();

		if (DIRECTILLUMINATION) {
			if (typeid(*hitObj) == typeid(Box)) {
				std::cerr << "Trying to render a Box (Bounding box?)" << std::endl;
			}

			if (hitObj->material.brdf != nullptr) {
				colour = our_getBDRF(lightVector, viewVector, hitNormal, material->brdf);
			}
			else {
				std::cerr << "NULLPTR for material BRDF." << std::endl;
			}
		}

		//Photon mapping
		if (PMAP) {
			const int num_photons = 400;
			auto photons = this->photonMap->findNearestNeighbours(intersectionPoint, num_photons); //Priority queue

			const float radiusSquared = (photons.top()->position - intersectionPoint).squaredNorm();

			Vec3 pmap_contrib = Vec3(0, 0, 0);

			while (photons.size()) {
				const Vec3 inVec = photons.top()->incomingAngle;
				const Vec3 photonBRDF = our_getBDRF(-inVec, viewVector, hitNormal, material->brdf);
				pmap_contrib += photons.top()->flux.cwiseProduct(photonBRDF);
				photons.pop();
			}

			const float multiplier = 20 / (M_PI * radiusSquared);
			pmap_contrib *= multiplier;
			colour += pmap_contrib;
		}
	}
	return colour;
}


void Scene::render()
{
	const int lines = camera->imagePlane.resolution(0);

	this->lineDone = (bool*)malloc(sizeof(bool) * lines);
	for (int i = 0; i < lines; i++) {
		lineDone[i] = false;
	}

	std::vector<std::thread> threads;

	const int numThreads = 8;
	for (int i = 0; i < numThreads; i++) {
		threads.push_back(std::thread(&Scene::tracerThread, this, i, numThreads));
	}

	int count = 0;
	int smallest = 0;

	while (smallest < lines) {
		for (int i = smallest; i < lines; i++) {
			if (lineDone[i] && smallest == i) {
				smallest++;
			}
		}

		//render
		cv::imshow("Render output", *this->target); // Show our image inside it.
		const int sKey = 115;
		if (cv::waitKey(1) == sKey) {
			cv::imwrite("C:/Users/mbuck/Desktop/output.png", *this->target);
		}
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}


	for (int i = 0; i < numThreads; i++) {
		threads[i].join();
	}
}

Scene::Scene()
{
}


Scene::~Scene()
{
	free(lineDone);
}
