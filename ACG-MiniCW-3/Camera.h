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

	Vec3 calculatePixelWorldPos(unsigned int pixelX, unsigned int pixelY);
	ImagePlane imagePlane;
	Camera();
	virtual ~Camera();
};

