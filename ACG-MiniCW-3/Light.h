#pragma once
#include "Vectors.h";

class Light
{
public:
	Vec3 colour;

	virtual Vec3 vectorTo(Vec3 position) = 0;

	Light(Vec3 colour) {
		this->colour = colour;
	};

	virtual ~Light() {};
};