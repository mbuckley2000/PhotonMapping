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

/**
 * Converts a colour from RGB(0,1) colour space to RGB(0,255)
 * Any values above 255 will be capped at 255
 */
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
			(*this->target)(y, x) = toRGB(this->traceRay(&ray, 0, 100) * dimmingFactor);
		}
		this->lineDone[x] = true;
	}
}

Vec3 Scene::getPMComponent(IntersectionContext context, int numberOfPhotons, PhotonMap* map, bool filter, float filterConst)
{
	auto photons = map->findNearestNeighbours(*context.intersectionPoint, numberOfPhotons); //Priority queue

	const float radiusSquared = (photons.top()->position - *context.intersectionPoint).squaredNorm();

	Vec3 pmapContrib = Vec3(0, 0, 0);

	while (photons.size()) {
		const Vec3 inVec = photons.top()->incomingAngle;
		const Vec3 photonBRDF = our_getBDRF(-inVec, *context.outboundVector, *context.surfaceNormal, context.material->brdf);
		Vec3 thisContrib = photons.top()->flux.cwiseProduct(photonBRDF);
		const float thisDist = (photons.top()->position - *context.intersectionPoint).squaredNorm();
		if (filter) {
			//Apply cone filtering
			thisContrib *= (1 - (thisDist / (radiusSquared*filterConst)));
		}
		pmapContrib += thisContrib;
		photons.pop();
	}

	const float multiplier = 1 / (M_PI * radiusSquared);

	return pmapContrib * multiplier;
}

Vec3 Scene::getDIComponent(IntersectionContext context)
{
	Vec3 colour = Vec3(0, 0, 0);

	if ((*context.material).brdf != nullptr) {
		colour = our_getBDRF(*context.inboundVector, *context.outboundVector, *context.surfaceNormal, context.material->brdf);
	}
	else {
		std::cerr << "NULLPTR for material BRDF." << std::endl;
	}

	return Vec3(colour);
}

Vec3 Scene::getTransmissiveComponent(IntersectionContext context, int depth, int maxDepth)
{
	Vec3 refractiveComponent(0, 0, 0);
	Vec3 reflectiveComponent(0, 0, 0);

	if (context.material->reflectiveness > 0) {
		Ray reflectiveRay = Ray(*context.intersectionPoint, reflectVector(-*context.inboundVector, *context.surfaceNormal));
		reflectiveRay.position += EPSILON * reflectiveRay.direction;
		reflectiveComponent = context.material->reflectiveness * this->traceRay(&reflectiveRay, depth + 1, maxDepth);
	}

	if (context.material->refractiveness > 0) {
		Ray refractiveRay = Ray(*context.intersectionPoint, refractVector(*context.inboundVector, *context.surfaceNormal, context.material->indexOfRefraction));
		refractiveRay.position += EPSILON * refractiveRay.direction;
		refractiveComponent = context.material->refractiveness * this->traceRay(&refractiveRay, depth + 1, maxDepth);
	}

	return reflectiveComponent + refractiveComponent;
}

Vec3 Scene::getShadowComponent(IntersectionContext context, int numberOfSamples)
{
	bool shadowed = false;
	int hitCount = 0;

	for (int i = 0; i < numberOfSamples; i++) {
		Ray shadowTest = Ray(*context.intersectionPoint, -light->vectorTo(*context.intersectionPoint));
		shadowTest.position = shadowTest.position + (EPSILON * shadowTest.direction);
		float shadowT;
		if (shadowTest.intersectsWith(*this, shadowT)) {
			if (shadowT <= (light->getDistanceFrom(*context.intersectionPoint))) {
				hitCount++;
				shadowed = true;
			}
		}
	}

	return Vec3(0.5 * hitCount, 0.5*hitCount, 0.5*hitCount);
}

//Returns colour
Vec3 Scene::traceRay(Ray* ray, int depth, int maxDepth) {
	const bool SHADOWING = false;
	const bool DIRECTILLUMINATION = true;
	const bool PMAP = true;

	Vec3 colour = Vec3(0, 0, 0);

	float t;
	Object* hitObj = nullptr;

	if (ray->intersectsWith(*this, hitObj, t)) {
		if (typeid(*hitObj) == typeid(Box)) {
			std::cerr << "Trying to render a Box (Bounding box?)" << std::endl;
		}

		//We have a hit
		const Vec3 normalisedRayDir = ray->direction.normalized();
		const Vec3 intersectionPoint = ray->position + (t * normalisedRayDir);
		Material* material = &(hitObj->material);
		Vec3 hitNormal = hitObj->getNormalAt(intersectionPoint);
		const Vec3 viewVector = (ray->position - intersectionPoint).normalized();

		//Used to pass these variables around
		IntersectionContext context;
		context.inboundVector = &normalisedRayDir;
		context.intersectionPoint = &intersectionPoint;
		context.material = material;
		context.surfaceNormal = &hitNormal;
		context.outboundVector = &viewVector;

		//Reflections & refractions
		if (material->transmissive && depth < maxDepth) {
			return this->getTransmissiveComponent(context, depth, maxDepth);
		}

		if (DIRECTILLUMINATION) {
			const Vec3 lightVector = -light->vectorTo(intersectionPoint).normalized();
			context.inboundVector = &lightVector;
			colour += this->getDIComponent(context);
		}

		if (PMAP) {
			//Indirect illumination
			colour += 10*this->getPMComponent(context, 100, this->photonMap, false, NULL);
			//Caustics
			colour += 1 * this->getPMComponent(context, 100, this->causticMap, true, 1);
		}

	}
	return colour;
}


void Scene::render()
{
	const int lines = camera->imagePlane.resolution(0);

	//Keep track of completed lines for GUI visualisation
	this->lineDone = (bool*)malloc(sizeof(bool) * lines);
	for (int i = 0; i < lines; i++) {
		lineDone[i] = false;
	}

	std::vector<std::thread> threads;

	//Split workload between threads
	//Each thread gets it's ID and the number of threads
	//It starts at col=ID, then increments by num_threads
	//Interlaced pattern
	//Spreads work
	const int numThreads = 8;
	for (int i = 0; i < numThreads; i++) {
		threads.push_back(std::thread(&Scene::tracerThread, this, i, numThreads));
	}

	int count = 0;
	int smallest = 0;

	//Update gui while rendering
	while (smallest < lines) {
		for (int i = smallest; i < lines; i++) {
			if (lineDone[i] && smallest == i) {
				smallest++;
			}
		}

		//update gui
		cv::imshow("Render output", *this->target); // Show our image inside it.
		const int sKey = 115;
		if (cv::waitKey(1) == sKey) {
			cv::imwrite("C:/Users/mbuck/Desktop/output.png", *this->target);
		}
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}

	//Wait for threads to finish
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