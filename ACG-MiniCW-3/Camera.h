#pragma once
#include <Eigen/Dense>

using namespace Eigen;

struct ImagePlane {
	Vector2d worldSize;
	Vector2i resolution;
};

class Camera
{
public:
	Vector3d focus; //Focal position

	//Direction vectors
	Vector3d up;
	Vector3d right;
	Vector3d backward;

	double focalLength;

	Vector3d calculatePixelWorldPos(unsigned int pixelX, unsigned int pixelY);
	ImagePlane imagePlane;
	Camera();
	virtual ~Camera();
};

