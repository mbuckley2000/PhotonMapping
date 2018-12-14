#pragma once
#include "Vectors.h";

class Light
{
public:
	Vec3 colour;

	/**
	 * Returns the vector from the light to the given position
	 */
	virtual Vec3 vectorTo(Vec3 position) = 0;

	/**
	 * Returns the distance of the light from the from the given position
	 * Used to check if the light is in front of or behind an object for computing shadows
	 */
	virtual float getDistanceFrom(Vec3 position) = 0;

	/**
	 * Returns the position of the light
	 */
	virtual Vec3 getPosition() = 0;

	/**
	 * Construct the light with the given colour
	 */
	Light(Vec3 colour) {
		this->colour = colour;
	};
};