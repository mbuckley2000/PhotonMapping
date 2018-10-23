#pragma once
#include <Eigen\Dense>

using namespace Eigen;

class Orientation
{
public:
	Orientation();
	virtual ~Orientation();

	Vector3f position;
	Vector3f up;
	Vector3f right;
	Vector3f backward;

};