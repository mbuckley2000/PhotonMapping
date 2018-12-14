#pragma once
#include "Light.h"
#include "Vectors.h"

class DirectionalLight :
	public Light
{
public:
	Vec3 direction;

	DirectionalLight(Vec3 direction, Vec3 colour);

	/**
	 * Constant for directional lights
	 */
	Vec3 vectorTo(Vec3 position);

	/**
	 * Always infinite for directional lights
	 */
	virtual float getDistanceFrom(Vec3 position) override;

	/**
	 * Meaningless for directional lights
	 */
	virtual Vec3 getPosition() override;

};

