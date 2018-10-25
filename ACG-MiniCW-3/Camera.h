#pragma once
#include <Eigen/Dense>

using namespace Eigen;

struct ImagePlane {
	Vector2f worldSize;
	Vector2i resolution;
};

class Camera
{
public:
	Vector3f focus; //Focal position

	//Direction vectors
	Vector3f up;
	Vector3f right;
	Vector3f backward;

	float focalLength;

	Vector3f calculatePixelWorldPos(unsigned int pixelX, unsigned int pixelY);
	ImagePlane imagePlane;
	Camera();
	virtual ~Camera();
};

