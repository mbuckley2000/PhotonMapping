#pragma once
#include "Light.h"
#include "Vectors.h"
#include <random>

/**
 * Square lights must be aligned with the x and z axes
 * Used to generate soft shadows
 */
class SquareLight :
	public Light
{
public:
	Vec3 position;
	Vec3 normal;
	float size;

	SquareLight(Vec3 colour, Vec3 position, Vec3 normal, float size);

	/**
	 * Calculated using the last sampled position from getPosition
	 */
	virtual Vec3 vectorTo(Vec3 position) override;

	/**
	 * Calculated using the last sampled position from getPosition
	 */
	virtual float getDistanceFrom(Vec3 position) override;

private:
	Vec3 lastPoint;
	std::default_random_engine generator;
	std::uniform_real_distribution<float> distribution;

	/**
	 * Returns a randomly sampled position from within the area of the light
	 */
	virtual Vec3 getPosition() override;
};