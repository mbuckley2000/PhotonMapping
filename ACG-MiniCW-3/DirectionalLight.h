#pragma once
#include "Light.h"
#include <Eigen/Dense>

class DirectionalLight :
	public Light
{
public:
	Eigen::Vector3f direction;

	DirectionalLight(Eigen::Vector3f, Eigen::Vector3f direction);

	Eigen::Vector3f vectorTo(Eigen::Vector3f position);

	virtual ~DirectionalLight();
};

