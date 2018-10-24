#include "pch.h"
#include "Camera.h"

/*
Calculates the world position of a pixel on the image plane of the camera
Lecture 4
*/
Vector3d Camera::calculatePixelWorldPos(unsigned int pixelX, unsigned int pixelY)
{
	const unsigned int i = pixelX;
	const unsigned int j = pixelY;
	const double W = this->imagePlane.worldSize(0);
	const double H = this->imagePlane.worldSize(1);
	const unsigned int nW = this->imagePlane.resolution(0);
	const unsigned int nH = this->imagePlane.resolution(1);

	const double r = W * (((j - 0.5) / nW) - 0.5);
	const double b = H * (((i - 0.5) / nH) - 0.5);

	const Vector3d e = this->focus;
	const double d = this->focalLength;
	const Vector3d w = this->backward;
	const Vector3d u = this->up;
	const Vector3d v = this->right;

	return( e + (d*-w) + (r*u) - (b*v));
}

Camera::Camera()
{
}


Camera::~Camera()
{
}
