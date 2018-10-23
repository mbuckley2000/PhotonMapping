#pragma once
#include <Eigen\Dense>

using namespace Eigen;

class ObjectInstance
{
public:
	ObjectInstance();
	virtual ~ObjectInstance();

	Vector3f position;
	Vector3f up;
	Vector3f rightDir;

};

