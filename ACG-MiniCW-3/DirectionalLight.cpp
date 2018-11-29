#include "pch.h"
#include "DirectionalLight.h"

DirectionalLight::DirectionalLight(Vec3 direction, Vec3 colour) : Light(colour)
{
	this->direction = direction.normalized();
}

Vec3 DirectionalLight::vectorTo(Vec3 position)
{
	return this->direction;
}


DirectionalLight::~DirectionalLight()
{
}

float DirectionalLight::getDistanceFrom(Vec3 position)
{
	return INFINITY;
}

Vec3 DirectionalLight::getPosition()
{
	return Vec3(INFINITY, INFINITY, INFINITY).cwiseProduct(-this->direction);
}

