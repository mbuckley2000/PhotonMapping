#include "pch.h"
#include "DirectionalLight.h"
#include <Eigen/Dense>

DirectionalLight::DirectionalLight(Eigen::Vector3f colour, Eigen::Vector3f direction) : Light(colour)
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
