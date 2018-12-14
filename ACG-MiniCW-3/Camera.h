#pragma once
#include "Vectors.h"

struct ImagePlane {
	Eigen::Vector2f worldSize;
	Eigen::Vector2i resolution;
};

class Camera
{
public:
	Vec3 focus; //Focal position

	//Direction vectors
	Vec3 up;
	Vec3 right;
	Vec3 backward;

	float focalLength;

	/**
	 * Calculates the world space position vector of a pixel on the camera's projection screen
	 */
	Vec3 calculatePixelWorldPos(unsigned int pixelX, unsigned int pixelY);

	ImagePlane imagePlane;
	Camera();
	virtual ~Camera();
};

