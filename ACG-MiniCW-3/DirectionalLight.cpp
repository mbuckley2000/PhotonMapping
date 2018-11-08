#include "pch.h"
#include "DirectionalLight.h"
#include <Eigen/Dense>

DirectionalLight::DirectionalLight(Eigen::Vector3f direction, Eigen::Vector3f colour) : Light(colour)
{
	this->direction = direction.normalized();
}

Eigen::Vector3f DirectionalLight::vectorTo(Eigen::Vector3f position)
{
	return this->direction;
}


DirectionalLight::~DirectionalLight()
{
}
