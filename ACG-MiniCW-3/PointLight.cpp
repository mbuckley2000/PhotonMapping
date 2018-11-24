#include "pch.h"
#include "PointLight.h"

PointLight::PointLight(Vec3 position, Vec3 colour) : Light(colour)
{
	this->position = position;
}

PointLight::~PointLight()
{
}

Vec3 PointLight::vectorTo(Vec3 position)
{
	return Vec3((position-this->position).normalized());
}

float PointLight::getDistanceFrom(Vec3 position)
{
	return (position - this->position).norm();
}

