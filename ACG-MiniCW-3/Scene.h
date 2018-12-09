#pragma once
#include "Camera.h"
#include "Object.h"
#include "Light.h"
#include <vector>
#include <opencv2/core.hpp>
#include "PhotonMap.h"

class Object;
class PhotonMap;

class Scene
{
public:
	std::vector<Object*> objects;
	Camera* camera;
	Light* light;
	cv::Mat_<cv::Vec3b>* target;
	PhotonMap* photonMap;

	void render();
	Scene();
	virtual ~Scene();

private:
	double* goldBDRF;
	Vec3 traceRay(Ray* ray, int depth, int maxDepth);
};

