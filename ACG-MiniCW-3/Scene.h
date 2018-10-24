#pragma once
#include "ObjectInstance.h"
#include "Camera.h"

class Scene
{
public:
	std::vector<ObjectInstance*> objectInstances;
	Camera* camera;

	void render(cv::Mat_<cv::Vec3b>& target);
	Scene();
	virtual ~Scene();
};

