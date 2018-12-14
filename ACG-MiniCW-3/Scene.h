#pragma once
#include "Camera.h"
#include "Object.h"
#include "Light.h"
#include <vector>
#include <opencv2/core.hpp>
#include "PhotonMap.h"

class Object;
class PhotonMap;

/**
 * Used to keep track of intersection parameters
 */
struct IntersectionContext {
	const Vec3* inboundVector;
	const Vec3* outboundVector;
	const Vec3* surfaceNormal;
	const Vec3* intersectionPoint;
	const Material* material;
};

class Scene
{
public:
	std::vector<Object*> objects;
	Camera* camera;
	Light* light;
	cv::Mat_<cv::Vec3b>* target;
	PhotonMap* photonMap;
	PhotonMap* causticMap;

	/**
	 * Renders the scene to the render target
	 */
	void render();

	Scene();
	virtual ~Scene();

private:
	bool* lineDone;

	/**
	 * Calculates the colour of a given ray after intersecting it and summing the individual components
	 */
	Vec3 traceRay(Ray* ray, int depth, int maxDepth);

	/**
	 * The runner method for each rendering thread
	 * Iterates over the render target, splitting it into numThreads chunks
	 */
	void tracerThread(int threadID, int numThreads);

	/**
	 * Calculates the colour of the photon mapping component for the given context
	 * Uses the scene's pre-calculated photon map
	 * Increasing numberOfPhotons with smoothen the result 
	 * Setting filter to true will apply a cone filter to sharpen caustics
	 * FilterConst affects the sharpness of the filtered caustic
	 * map is the pointer to the desired photon map
	 */
	Vec3 getPMComponent(IntersectionContext context, int numberOfPhotons, PhotonMap* map, bool filter, float filterConst);

	/**
	 * Calculates the colour of the direct illumination component for the given context
	 * Uses the MERL BRDF library and the object's material
	 */
	Vec3 getDIComponent(IntersectionContext context);

	/**
	 * Calculates the colour of the transmissive component for the given context
	 * This is the sum of the reflective and refractive components for transmissive objects
	 * Based on the object material
	 */
	Vec3 getTransmissiveComponent(IntersectionContext context, int depth, int maxDepth);

	/**
	 * Calculates the shadow component for the given context
	 * Sum of randomly sampled shadow rays on an area light
	 */

	Vec3 getShadowComponent(IntersectionContext context, int numberOfSamples);
};

