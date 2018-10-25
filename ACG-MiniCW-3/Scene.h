#pragma once
#include "Camera.h"
#include <thread>
#include "Object.h"

class Scene
{
public:
	std::vector<Object*> objects;
	Camera* camera;
	cv::Mat_<cv::Vec3b>* target;

	void render();
	Scene();
	virtual ~Scene();
};

