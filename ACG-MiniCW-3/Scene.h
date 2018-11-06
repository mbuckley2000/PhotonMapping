#pragma once
#include "Camera.h"
#include <thread>
#include "Object.h"
#include "Light.h"

class Scene
{
public:
	std::vector<Object*> objects;
	Camera* camera;
	Light* light;
	cv::Mat_<cv::Vec3b>* target;

	void render();
	Scene();
	virtual ~Scene();
};

