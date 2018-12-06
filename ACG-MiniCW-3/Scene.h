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

	void render(PhotonMap* p);
	Scene();
	virtual ~Scene();
};

