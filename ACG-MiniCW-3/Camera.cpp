#include "pch.h"
#include "Camera.h"

Vec3 Camera::calculatePixelWorldPos(unsigned int pixelX, unsigned int pixelY)
{
	//Get parameters
	const unsigned int i = pixelX;
	const unsigned int j = pixelY;
	const float W = this->imagePlane.worldSize(0);
	const float H = this->imagePlane.worldSize(1);
	const unsigned int nW = this->imagePlane.resolution(0);
	const unsigned int nH = this->imagePlane.resolution(1);

	const Vec3 e = this->focus;
	const float d = this->focalLength;
	const Vec3 w = this->backward;
	const Vec3 u = this->up;
	const Vec3 v = this->right;

	//Formula from Lecture 4
	//Start at focus, move forward to plane, add / subtract in x and y to get to the pixel
	const float r = W * (((j - 0.5) / nW) - 0.5);
	const float b = H * (((i - 0.5) / nH) - 0.5);
	return( e + (d*-w) - (r*u) + (b*v));
}

Camera::Camera()
{
}


Camera::~Camera()
{
}
