#include "pch.h"
#include "SquareLight.h"
#include "Common.h"
SquareLight::SquareLight(Vec3 colour, Vec3 position, Vec3 normal, float size) : Light(colour)
{
	this->position = position;
	this->normal = normal.normalized();
	this->size = size;
}

SquareLight::~SquareLight()
{
}

Vec3 SquareLight::vectorTo(Vec3 position)
{
	//Get random point in the square
	//Direction vector of the plane
	const float halfSize = this->size / 2;
	const float xRand = random(-halfSize, halfSize);
	const float zRand = random(-halfSize, halfSize);

	Vec3 point = this->position;
	point(0) += xRand;
	point(2) += zRand;

	this->lastPoint = point;

	return (position - point).normalized();
}

float SquareLight::getDistanceFrom(Vec3 position)
{
	return (this->lastPoint - position).norm();
}
