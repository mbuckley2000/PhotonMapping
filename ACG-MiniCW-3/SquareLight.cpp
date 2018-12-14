#include "pch.h"
#include "SquareLight.h"
#include "Common.h"

SquareLight::SquareLight(Vec3 colour, Vec3 position, Vec3 normal, float size) : Light(colour)
{
	this->position = position;
	this->normal = normal.normalized();
	this->size = size;

	const float halfSize = this->size / 2;

	this->distribution = std::uniform_real_distribution<float>(-halfSize, halfSize);
}

Vec3 SquareLight::vectorTo(Vec3 position)
{
	//Uses last generated point from getPosition()
	const Vec3 point = this->getPosition();
	this->lastPoint = point;

	return (position - point).normalized();
}

float SquareLight::getDistanceFrom(Vec3 position)
{
	//Uses last generated point from getPosition()
	return (this->lastPoint - position).norm();
}

Vec3 SquareLight::getPosition()
{
	//Get random point in the square
	const float xRand = this->distribution(this->generator);
	const float zRand = this->distribution(this->generator);
	
	Vec3 point = this->position;
	point(0) += xRand;
	point(2) += zRand;

	return point;
}
